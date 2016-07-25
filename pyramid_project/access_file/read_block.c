#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

/*
 * Function: read_block
 *
 * Description: This function will attempt a "noisy" read of the desired block
 *  by going through each level of pyra and either finding the desired block
 *  and reading it, or failing to find it and reading a randomly selected
 *  block.  This function, when it finds the desired block ID in pyra, moves it
 *  to the top level of pyra.
 *
 * Inputs:
 *  file_fd - The file descriptor of the file being accessed.
 *  block - The identifier of the block to be accessed.
 *  pyra - A pointer to the top level of the pyramid for the file being
 *   accessed.
 *
 * Outputs:
 *  block_ids - If the function succeeds, this array will store the block IDs
 *   associated with the blocks stored in big_buffer.  The block with ID
 *   block_ids[i] is stored at big_buffer + BLOCK_SIZE * i.  THIS ARRAY IS
 *   ASSUMED TO BE LARGE ENOUGH TO STORE AS MANY LONGS AS THERE ARE LAYERS IN
 *   PYRA.
 *  big_buffer - If the function succeeds, this buffer will be used to store
 *   copies of each block accessed, one for each non-empty layer of pyra.  THIS
 *   BUFFER IS ASSUMED TO BE LARGE ENOUGH TO STORE AS MANY BLOCKS AS THERE ARE
 *   LAYERS IN PYRA.
 *
 * Return value:
 *  Upon success, this function returns a nonnegative number that is the offset
 *   (in bytes) from the beginning of big_buffer at which the requested block
 *   begins.
 *  -1 if move_id_in_pyramid fails.
 *  -2 if randomly_select_id_pyramid fails.
 *  -3 if any of the function inputs is invalid.
 *  -4 if a call to read caused an error; check errno.
 *  -5 if a call to read caused fewer bytes to be read than expected.
 *  -6 if a call to read caused more bytes to be read than expected.
 *  -7 if a call to fstat failed; check errno.
 *  -8 if the id "block" was not found in pyra.
 *  -9 if a call to lseek failed; check errno.
 */
ssize_t read_block(int file_fd, long block, pyramid *pyra, long *block_ids,
                    unsigned char *big_buffer) {
  
  if ((block < 0L) || !pyra || !block_ids || (file_fd < 0) || !big_buffer)
    return -3;
  int movereturn, notfoundyet, i, j;
  pyramid *this_level;
  ssize_t offset, readreturn, returnoffset;
  struct stat filestat;
  long final_block, this_block;

  notfoundyet = 1;
  returnoffset = -8;
  for (this_level = pyra, offset = 0, i = 0; this_level != NULL;
        this_level = this_level->next_level, offset += BLOCK_SIZE, ++i) {
    if (notfoundyet && ((movereturn = move_id_in_pyramid(this_level, pyra, block)) == 1)) {
      notfoundyet = 0;
      this_block = block;
    }
    else {
      if ((movereturn != -3) && notfoundyet) {
        return -1;
      }
      this_block = randomly_select_id_pyramid(this_level);
      if (this_block < 0L) {
        return -2;
      }
    }
    
    if (lseek(file_fd, this_block * BLOCK_SIZE, SEEK_SET) < 0) {
      return -9;
    }
    readreturn = read(file_fd, big_buffer + offset, BLOCK_SIZE);
    if (readreturn < 0) return -4;
    if (readreturn > BLOCK_SIZE) return -6;
    if (fstat(file_fd, &filestat)) return -7;
    if (!filestat.st_size) final_block = 0;
    else final_block = (long) ((filestat.st_size - (off_t) 1) / (off_t) BLOCK_SIZE);
    if ((this_block == final_block) && (filestat.st_size % BLOCK_SIZE)) {
      for (j = offset + (filestat.st_size % BLOCK_SIZE); j < offset + BLOCK_SIZE; ++j) big_buffer[j] = (unsigned char) 0;
      if (filestat.st_size % (off_t) BLOCK_SIZE != (off_t) (readreturn % BLOCK_SIZE)) {
        return -5;
      }
    }
    else {
      if (readreturn != BLOCK_SIZE) {
        return -5;
      }
    }
    if (this_block == block) {
      returnoffset = offset;
    }
    block_ids[i] = this_block;
  }
  
  return returnoffset;
}
