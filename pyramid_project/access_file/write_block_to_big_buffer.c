#include <strings.h>

#include "../pyramid/pyramid.h"

void write_block_to_big_buffer(unsigned char *writebuf, long first_block, long last_block, long requested_block, unsigned char *big_buffer, size_t count, size_t big_buffer_offset, off_t original_position) {
  size_t size_of_first = BLOCK_SIZE - (original_position % BLOCK_SIZE);
  unsigned num_blocks = last_block - first_block + 1;
  size_t size_of_last = count - (size_of_first + ((num_blocks - 2) * BLOCK_SIZE));

  if (first_block == last_block) {
    bcopy(writebuf, big_buffer + big_buffer_offset + (original_position % BLOCK_SIZE), count);
  }
  else if (requested_block == last_block) {
    bcopy(writebuf + (count - size_of_last), big_buffer + big_buffer_offset, size_of_last);
  }
  else if (requested_block == first_block) {
    bcopy(writebuf, big_buffer + big_buffer_offset + (original_position % BLOCK_SIZE), size_of_first);
  }
  else {
    bcopy(writebuf + size_of_first + (requested_block - first_block - 1) * BLOCK_SIZE, big_buffer + big_buffer_offset, BLOCK_SIZE);
  }
}
