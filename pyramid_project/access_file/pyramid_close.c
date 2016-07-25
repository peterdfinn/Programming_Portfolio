#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

#include <stdio.h>//DEBUG

/*
 * Function: pyramid_close
 *
 * Description: This function closes the file descriptor fd, encrypts the
 *  associated pyramid structure, writes the encrypted pyramid to the file
 *  "[filename]_pyramid", closes this file, and frees the pyramid.
 *
 * Inputs:
 *  fd - The file descriptor of the file you wish to close.
 *  pyra - A pointer to the top level of the pyramid associated with this file
 *   descriptor.
 *
 * Return value:
 *  0 upon success.
 *  -1 if the attempt to close fd failed; check errno.
 *  -2 if the attempt to close the pyramid file failed; check errno.
 *  -3 if pyra is NULL.
 *  -4 if pyra is not the top level of its pyramid.
 *  -5 if a call to sizeof_pyramid failed.
 *  -6 if a call to encrypt_pyramid failed.
 *  -7 if fd is invalid (it's negative, or it's reserved for stdin, stdout, or
 *   stderr).
 *  -8 if a call to snprintf failed.
 *  -9 if a call to open failed; check errno.
 *  -10 if a call to write didn't write as many bytes as was expected.
 *  -11 if a call to sign_pyramid failed.
 *  -12 if a call to ftruncate failed; check errno.
 *  -13 if a call to lseek failed; check errno.
 */
int pyramid_close(int fd, pyramid* pyra) {

  // If any of the inputs are invalid, return immediately.
  if (fd < 3) return -7;
  if (!pyra) return -3;
  if (pyra->level != 1) return -4;

  // Erase the contents of the current pyramid file.
  if (ftruncate(pyra->pyramid_fd, 0)) {
    return -12;
  }
  if (lseek(pyra->pyramid_fd, 0, SEEK_SET)) {
    return -13;
  }

  // Determine how many identifiers are capable of being stored in the input
  // pyramid, returning immediately in the event of an error in doing so.
  size_t pyramid_size = sizeof_pyramid(pyra);
  if (!pyramid_size) return -5;

  // Allocate space for the block IDs to be copied from the pyramid and the
  // encrypted version of the block IDs, based on the size of the pyramid.
  // Also, allocate space for the randomly generated nonce to be used in the
  // encryption of the pyramid.
  long identifiers[pyramid_size], encrypted_identifiers[pyramid_size];
  unsigned char nonce[KEY_LENGTH];

  // Copy the pyramid's block IDs to 'identifiers'.
  size_t offset = 0L;
  size_t num_ids = 4L;
  pyramid* p = pyra;
  while (p) {
    bcopy(p->identifiers, identifiers + offset, sizeof(long) * num_ids);
    offset += num_ids;
    num_ids = num_ids << 2;
    p = p->next_level;
  }
  
  // Encrypt 'identifiers' and store it in 'encrypted_identifiers', and store
  // the nonce in 'nonce', while checking for errors.
  int enc_return = encrypt_pyramid(identifiers, encrypted_identifiers, pyra, (int) (pyramid_size * sizeof(long)), nonce, (long) rand());
  if (enc_return < 0) return -6;

  // Append nonce to beginning of encrypted pyramid in to_write.
  unsigned long write_length = KEY_LENGTH + (pyramid_size * sizeof(long)) + SIGLEN;
  char to_write[write_length];
  bcopy(nonce, to_write + SIGLEN, KEY_LENGTH);
  bcopy(encrypted_identifiers, to_write + SIGLEN + KEY_LENGTH,
        pyramid_size * sizeof(long));

  // Sign pyramid file.
  char sig[SIGLEN];
  unsigned siglen;
  if (sign_pyramid(to_write + SIGLEN, write_length - SIGLEN, sig, &siglen))
    return -11;

  // Append signature to beginning of nonce and write buffer to file.
  bcopy(sig, to_write, SIGLEN);
  if (write(pyra->pyramid_fd, to_write, write_length) < write_length) return -10;


  int file_close_return = close(fd);
  if (file_close_return) return -1;
  int pyramid_close_return = close(pyra->pyramid_fd);
  if (pyramid_close_return) return -2;

  return 0;
}
