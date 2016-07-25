#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../pyramid/pyramid.h"

int main() {
  char* filename = "readblocktestfile";
  create_big_file(filename);
  unsigned char key[KEY_LENGTH];
  unsigned char *big_buffer = malloc(BLOCK_SIZE * 3);
  long block_ids[3];
  int i, file_fd;
  ssize_t read_block_return;
  for (i = 0; i < KEY_LENGTH; ++i) key[i] = (unsigned char) (rand() % 256);
  pyramid *p = openit(filename, &file_fd, key, 4);
  for (i = 1; i <= 3; ++i) {
    print_ids(p, i);
    long block = ((rand() % 23) + 6) % 23;
    printf("For test %d, reading block %ld\n", i, block);
    read_block_return = read_block(file_fd, block, p, block_ids, big_buffer);
    if (read_block_return < 0) {
      printf("error on iteration %d; read_block returned %ld\n", i, read_block_return);
      free(big_buffer);
      closeit(file_fd, p, i);
      return -1;
    }
    if ((read_block_return % BLOCK_SIZE) || (read_block_return / BLOCK_SIZE > 2)) {
      printf("strange error on iteration %d; read_block returned %ld\n", i, read_block_return);
      free(big_buffer);
      closeit(file_fd, p, i);
      return -1;
    }
    
    unsigned char block_buffer[BLOCK_SIZE];
    if (lseek(file_fd, block * BLOCK_SIZE, SEEK_SET) < 0) {
      printf("lseek failed on iteration %d\n", i);
      break;
    }
    if (read(file_fd, block_buffer, BLOCK_SIZE) < 0) {
      printf("read failed on iteration %d\n", i);
      break;
    }
    if (strncmp(big_buffer + read_block_return, block_buffer, BLOCK_SIZE)) {
      printf("inconsistent read on iteration %d for block %ld\n", i, block);
      free(big_buffer);
      closeit(file_fd, p, i);
      return -1;
    }
    print_ids(p, i);
  }
  printf("read_block_test passed!\n");
  closeit(file_fd, p, 4);
  free(big_buffer);
  return 0;
}
