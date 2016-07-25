#include <stdio.h>

#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

/*
 * Function: pyramid_read
 * 
 * Description: This function attempts to read count bytes from the file with
 *  file descriptor fd.  Upon success, the file position is incremented by
 *  the number of bytes read.  Upon failure, the file position is unchanged.
 *
 * Inputs:
 *  fd - The file descriptor of the file to read from.
 *  count - The number of bytes to be read.
 *  pyra - The pyramid struct associated with the file to read from.
 *
 * Outputs:
 *  buf - A buffer into which to store the bytes read from the file.  IT IS
 *   ASSUMED THAT BUF IS LARGE ENOUGH TO STORE THE BYTES.
 *
 * Return value:
 *  The number of bytes read if the function succeeds.
 *  -1 if any of the function's inputs is invalid.
 *  -2 if a call to read_block failed.
 *  -3 if a call to lseek failed; check errno.
 *  -4 if malloc failed; check errno.
 */
ssize_t pyramid_read(int fd, unsigned char *buf, size_t count, pyramid *pyra) {
  if (!pyra || (fd < 0) || !buf) return -1L;

  long first_block_id, last_block_id, curr_block_id;
  unsigned num_levels = how_many_levels(pyra);
  long block_ids[num_levels];
  ssize_t read_block_return;
  off_t original_position = lseek(fd, (off_t) 0, SEEK_CUR);
  size_t buf_offset = 0L;
  if (original_position < 0) return -3L;
  unsigned char *big_buffer = malloc(num_levels * BLOCK_SIZE);
  if (!big_buffer) return -4L;
  
  determine_blocks(count, &first_block_id, &last_block_id, original_position);
  for (curr_block_id = first_block_id; curr_block_id <= last_block_id; ++curr_block_id) {
    read_block_return = read_block(fd, curr_block_id, pyra, block_ids, big_buffer);
    //printf("in pyramid_read: after read_block(%ld), big_buffer contains %c%c%c%c\n", curr_block_id, *(big_buffer+read_block_return+56), *(big_buffer+read_block_return+57), *(big_buffer+read_block_return+58), *(big_buffer+read_block_return+59));//DEBUG
    
    if (read_block_return < 0L) {
      free(big_buffer);
      lseek(fd, original_position, SEEK_SET);
      return -2L;
    }
    
    if (first_block_id == last_block_id) {
      bcopy(big_buffer + read_block_return + (original_position % BLOCK_SIZE), buf, count);
      buf_offset += count;
    }
    else if (curr_block_id == first_block_id) {
      size_t this_size = BLOCK_SIZE - (original_position % BLOCK_SIZE);
      bcopy(big_buffer + read_block_return + (original_position % BLOCK_SIZE), buf, this_size);
      buf_offset += this_size;
      //printf("in pyramid_read: buf contains %c%c%c%c%c\n", buf[1], buf[2], buf[3], buf[4], buf[5]);//DEBUG
    }
    else if (curr_block_id == last_block_id) {
      size_t this_size = count - buf_offset;
      bcopy(big_buffer + read_block_return, buf + buf_offset, this_size);
      buf_offset += this_size;
    }
    else {
      bcopy(big_buffer + read_block_return, buf + buf_offset, BLOCK_SIZE);
      buf_offset += BLOCK_SIZE;
    }
  }

  free(big_buffer);
  if (lseek(fd, original_position + buf_offset, SEEK_SET) < 0) return -3L;
  return (ssize_t) buf_offset;
}
