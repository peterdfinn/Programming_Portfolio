#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

/*
 * Function: pyramid_write
 *
 * Description: This function attempts to write count bytes from buf to the
 *  file associated with fd.  Whenever a block is written, its ID is moved to
 *  the top level of pyra.  If the file is not currently large enough to
 *  support the write given its current position and the value of count, the
 *  file is first resized with ftruncate to make it large enough.  If
 *  pyramid_write succeeds, the position of the file is set to its current
 *  position + count.  If it fails, the position of the file is set to its
 *  current position.
 *
 * Inputs:
 *  fd - The file descriptor of the file.
 *  buf - A buffer containing the data to be written to the file.  IS ASSUMED
 *   TO BE AT LEAST COUNT BYTES IN LENGTH.
 *  count - The number of bytes to write to the file.
 *  pyra - A pointer to the pyramid struct associated with this file.
 *
 * Return value:
 *  0 upon success.
 *  -1 if any of pyramid_write's inputs is invalid.
 *  -2 if fstat failed; check errno.
 *  -3 if add_id_pyramid failed.
 *  -4 if ftruncate failed; check errno.
 *  -5 if malloc failed; check errno.
 *  -6 if read_block failed.
 *  -7 if lseek failed; check errno.
 *  -8 if write failed; check errno.
 */
ssize_t pyramid_write(int fd, unsigned char *buf, size_t count, pyramid *pyra) {
  if ((fd < 0) || !buf || !pyra) return -1L;
  if (count == 0L) return 0L;
  
  struct stat filestat;
  if (fstat(fd, &filestat)) {
    return -2L;
  }

  long final_block;
  if (filestat.st_size == 0) {
    final_block = -1;
  }
  else {
    if ((filestat.st_size % (off_t) BLOCK_SIZE) == 0) {
      final_block = (long) ((filestat.st_size / (off_t) BLOCK_SIZE) - (off_t) 1);
    }
    else {
      final_block = (long) (filestat.st_size / (off_t) BLOCK_SIZE);
    }
  }

  int would_create_blocks;
  long new_final_block;
  off_t original_position, new_size;
  original_position = lseek(fd, 0, SEEK_CUR);
  new_size = original_position + (off_t) count;
  if ((new_size % (off_t) BLOCK_SIZE) == (off_t) 0) {
    new_final_block = (long) ((new_size / (off_t) BLOCK_SIZE) - (off_t) 1);
  }
  else {
    new_final_block = (long) (new_size / (off_t) BLOCK_SIZE);
  }
  would_create_blocks = (new_final_block > final_block);
  
  long i;
  if (would_create_blocks) {
    for (i = final_block + 1; i <= new_final_block; ++i) {
      if (!add_id_pyramid(i, pyra)) {
        return -3L;
      }
    }
    if (ftruncate(fd, (off_t) ((new_final_block + 1) * BLOCK_SIZE))) {
      return -4L;
    }
  }

  long first_block_id, last_block_id;
  determine_blocks(count, &first_block_id, &last_block_id, original_position);

  unsigned num_levels;
  num_levels = how_many_levels(pyra);
  long block_ids[num_levels];
  unsigned char *big_buffer = malloc(BLOCK_SIZE * num_levels);
  if (!big_buffer) {
    return -5L;
  }

  long curr_block_id;
  ssize_t read_block_return;
  for (curr_block_id = first_block_id; curr_block_id <= last_block_id; ++curr_block_id) {
    read_block_return = read_block(fd, curr_block_id, pyra, block_ids, big_buffer);
    if (read_block_return < 0) {
      free(big_buffer);
      lseek(fd, original_position, SEEK_SET);
      return -6L;
    }

    write_block_to_big_buffer(buf, first_block_id, last_block_id, curr_block_id, big_buffer, count, read_block_return, original_position);

    size_t big_buffer_offset;
    for (i = 0, big_buffer_offset = 0; i < num_levels; ++i, big_buffer_offset += BLOCK_SIZE) {
      if (lseek(fd, block_ids[i] * BLOCK_SIZE, SEEK_SET) < 0) {
        free(big_buffer);
        lseek(fd, original_position, SEEK_SET);
        return -7L;
      }
      if (write(fd, big_buffer + big_buffer_offset, BLOCK_SIZE) < 0) {
        free(big_buffer);
        lseek(fd, original_position, SEEK_SET);
        return -8L;
      }
    }
  }
}
