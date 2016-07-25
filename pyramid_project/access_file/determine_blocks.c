#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

/*
 * Function: determine_blocks
 *
 * Description: This function determines which block(s) in the file are to be
 *  accessed.
 *
 * Inputs:
 *  count - The number of bytes requested for reading/writing.
 *  position - The current position of the file.
 *
 * Outputs:
 *  first - The number of the first block of the range encompassing the
 *   requested bytes.
 *  last - The number of the last block of the range encompassing the requested
 *   bytes.
 */
void determine_blocks(size_t count, long *first, long *last, off_t position) {
  *first = (long) (position / (off_t) BLOCK_SIZE);
  *last = (long) ((position + (off_t) (count - 1)) / (off_t) BLOCK_SIZE);
  return;
}
