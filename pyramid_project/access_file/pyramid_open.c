#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

/*
 * Function: pyramid_open
 *
 * Description: This function attempts to open the file referenced by filename
 *  and the associated pyramid file [filename]_pyramid, if it exists, creating
 *  it if it does not exist.  It returns a pointer to a newly allocated and
 *  initialized pyramid struct upon success.
 *
 * Inputs:
 *  pathname - The pathname of the file to be passed to open().
 *  flags - The flags to be passed to open(); see manual entry.
 *  key - An array of size KEY_LENGTH containing the key needed to unlock the
 *   files.
 *
 * Outputs:
 *  file_fd - A pointer into which the return value of opening the file will be
 *   stored.
 *  pyramid_fd - A pointer into which the return value of opening the pyramid
 *   file will be stored.
 *  err: An error value, indicating one of the following:
 *   0 if there were no errors.
 *   -1 if the attempt to open the file failed; check errno.
 *   -2 if the attempt to open the pyramid file failed; check errno.
 *   -3 if a call to pyramid_init failed.
 *   -4 if fstat failed; check errno.
 *   -5 if any attempt to add a block ID into the pyramid struct failed.
 *   -6 if read returned -1; check errno.
 *   -7 if read didn't return an error, but did read fewer bytes than expected.
 *   -8 if decrypt_pyramid failed.
 *   -9 if verify_pyramid indicates that the pyramid file's integrity has been
 *    compromised.
 *   -10 if verify_pyramid failed for some other reason.
 *   -11 if fchmod failed; check errno.
 *   
 * Return value:
 *  NULL upon failure; check err.
 *  Otherwise, returns a pointer to the pyramid struct associated with the file
 *   referenced by pathname.
 */
pyramid* pyramid_open(char *pathname, int flags, int* file_fd, int* pyramid_fd, int *err, unsigned char *key) {
  *err = 0;

  // Open the file itself.
  *file_fd = open(pathname, flags);
  if (*file_fd < 0) {
    *err = -1;
    return NULL;
  }
  
  // Open the pyramid file.
  size_t pathname_len = strlen(pathname);
  char pyramid_filename[pathname_len + 10];
  pyramid_filename[pathname_len + 9] = 0;
  snprintf(pyramid_filename, pathname_len + 10, "%s_pyramid", pathname);
  *pyramid_fd = open(pyramid_filename, O_EXCL|O_CREAT|O_RDWR);
  int new_pyramid = 1;
  if ((*pyramid_fd < 0) && (errno == EEXIST)) {
    *pyramid_fd = open(pyramid_filename, O_RDWR);
    new_pyramid = 0;
  }
  if (*pyramid_fd < 0) {
    *err = -2;
    return NULL;
  }

  // Allocate and initialize pyramid.  If no pyramid file existed, fill in the
  // pyramid with the block IDs in arbitrary order.
  pyramid *p = pyramid_init(NULL, *pyramid_fd, key);
  if (!p) {
    *err = -3;
    return NULL;
  }
  if (new_pyramid) {
    if (fchmod(*pyramid_fd, S_IRUSR|S_IWUSR)) {
      pyramid_free(p);
      *err = -11;
      return NULL;
    }
    struct stat filestat;
    if (fstat(*file_fd, &filestat)) {
      pyramid_free(p);
      *err = -4;
      return NULL;
    }
    ssize_t filesize = filestat.st_size;
    long num_blocks = filesize / BLOCK_SIZE;
    if (num_blocks * BLOCK_SIZE != filesize) num_blocks++;
    long i;
    for (i = 0L; i < num_blocks; ++i) {
      if (!add_id_pyramid(i, p)) {
        pyramid_free(p);
        *err = -5;
        return NULL;
      }
    }
    return p;
  }

  // Define signature, verification payload, nonce, and decryption payload of
  // pyramid file.
  unsigned char signature[SIGLEN];
  struct stat pyramid_filestat;
  if (fstat(*pyramid_fd, &pyramid_filestat)) {
    pyramid_free(p);
    *err = -4;
    return NULL;
  }
  ssize_t pyramid_filesize = pyramid_filestat.st_size - SIGLEN - KEY_LENGTH;
  unsigned char verification_payload[pyramid_filesize + KEY_LENGTH];
  unsigned char *nonce = verification_payload;
  unsigned char *decryption_payload = verification_payload + KEY_LENGTH;

  // Get signature and payload from pyramid file and verify pyramid file.
  ssize_t readreturn = read(*pyramid_fd, signature, SIGLEN);
  if (readreturn != SIGLEN) {
    pyramid_free(p);
    if (readreturn < 0) *err = -6;
    else *err = -7;
    return NULL;
  }
  readreturn = read(*pyramid_fd, verification_payload, pyramid_filesize + KEY_LENGTH);
  if (readreturn != pyramid_filesize + KEY_LENGTH) {
    pyramid_free(p);
    if (readreturn < 0) *err = -6;
    else *err = -7;
    return NULL;
  }
  int verifyreturn = verify_pyramid(signature, verification_payload, pyramid_filesize + KEY_LENGTH);
  if (verifyreturn < 0) {
    pyramid_free(p);
    if (!verifyreturn) *err = -9;
    else *err = -10;
    return NULL;
  }

  // Decrypt pyramid file into decrypted_identifiers.
  long decrypted_identifiers[pyramid_filesize / sizeof(long)];
  if (decrypt_pyramid(key, nonce, decryption_payload, decrypted_identifiers, pyramid_filesize) < 0) {
    pyramid_free(p);
    *err = -8;
    return NULL;
  }

  // Count the number of layers of the pyramid.
  int counter = 0;
  long num_ids = 4L;
  while (num_ids < pyramid_filesize / sizeof(long)) {
    counter++;
    num_ids *= 4L;
  }

  // Copy the decrypted identifiers into a pyramid structure.
  int i;
  int offset = 0;
  pyramid* this = p;
  bcopy(decrypted_identifiers + offset, this->identifiers, how_many_ids(this) * sizeof(long));
  offset += how_many_ids(this);
  for (i = 1; i < counter; ++i) {
    if (!pyramid_init(this, *pyramid_fd, key)) {
      pyramid_free(p);
      *err = -3;
      return NULL;
    }
    this = this->next_level;
    bcopy(decrypted_identifiers + offset, this->identifiers, how_many_ids(this) * sizeof(long));
    offset += how_many_ids(this);
  }
  
  return p;
}
