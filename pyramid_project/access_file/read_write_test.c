#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"

void read_write_test_remainder(unsigned char *key);

int main() {
  int file_fd, pyramid_fd, err, i;
  unsigned char key[KEY_LENGTH];
  for (i = 0; i < KEY_LENGTH; ++i)
    key[i] = (unsigned char) (255 - (rand() % 256));
  pyramid *test_pyramid = pyramid_open("readwritetestfile",
                                        O_CREAT|O_RDWR|O_TRUNC, &file_fd, &pyramid_fd,
                                        &err, key);
  if (!test_pyramid) {
    printf("pyramid_open failed; file_fd=%d; err = %d\n", file_fd, err);
    printf("%s\n", strerror(errno));
  }

  unsigned char eclipse[49];
  bcopy("Nothing I could do, @ total eclipse of the heart.", eclipse, 49);
  unsigned char read_back[49];
  ssize_t readret, writeret;
  lseek(file_fd, 20, SEEK_SET);
  writeret = pyramid_write(file_fd, eclipse + 20, 29, test_pyramid);
  lseek(file_fd, 20, SEEK_SET);
  readret = pyramid_read(file_fd, read_back + 20, 29, test_pyramid);
  if (strncmp(eclipse + 20, read_back + 20, 29)) {
    printf("test 1 failed\nshould read: %s\nactually reads: %s\n", eclipse + 20, read_back + 20);
    printf("pyramid_write returned %ld, pyramid_read returned %ld\n", writeret, readret);
    pyramid_close(file_fd, test_pyramid);
    return -1;
  }

  eclipse[20] = 'a';
  lseek(file_fd, 0, SEEK_SET);
  pyramid_write(file_fd, eclipse, 21, test_pyramid);
  lseek(file_fd, 0, SEEK_SET);
  pyramid_read(file_fd, read_back, 49, test_pyramid);
  if (strncmp(eclipse, read_back, 49)) {
    printf("test 2 failed\nshould read: %s\nactually reads: %s\n", eclipse, read_back);
    pyramid_close(file_fd, test_pyramid);
    return -1;
  }

  printf("tests passed!\n");
  fchmod(file_fd, S_IRUSR|S_IWUSR);
  fchmod(pyramid_fd, S_IRUSR|S_IWUSR);
  pyramid_close(file_fd, test_pyramid);

  read_write_test_remainder(key);

  return 0;
}

void read_write_test_remainder(unsigned char *key) {
  int file_fd, pyramid_fd, err;
  ssize_t writeret, readret;
  pyramid *remainder_pyramid = pyramid_open("readwritetestfile", O_RDWR, &file_fd, &pyramid_fd, &err, key);
  lseek(file_fd, 48, SEEK_SET);
  unsigned char *writebuf = malloc(45 * BLOCK_SIZE - 48);
  writebuf[0] = (unsigned char) '\n';
  int i;
  for (i = 1; i < 45 * BLOCK_SIZE - 49; ++i) {
    if (i % 27 == 0) writebuf[i] = (unsigned char) 'a';
    else writebuf[i] = (unsigned char) 'a';//96 + (i % 27);
  }
  writebuf[45 * BLOCK_SIZE - 49] = (unsigned char) EOF;
  writeret = pyramid_write(file_fd, writebuf, 45 * BLOCK_SIZE - 48, remainder_pyramid);
  printf("in test 3, pyramid_write returned %ld\n", writeret);//DEBUG
  unsigned char *readbuf = malloc(45 * BLOCK_SIZE - 500);
  lseek(file_fd, 48, SEEK_SET);
  readret = pyramid_read(file_fd, readbuf, 45 * BLOCK_SIZE - 500, remainder_pyramid);
  printf("in test 3, pyramid_read returned %ld\n", readret);//DEBUG
  if (strncmp(readbuf, writebuf, 45 * BLOCK_SIZE - 500)) {
    //for (i=0; i<101*BLOCK_SIZE-500; ++i) if (readbuf[i]!=writebuf[i]) printf("index %d: should be %d, is %d\n", i, (int)writebuf[i], (int)readbuf[i]);//DEBUG
    printf("test 3 failed\n");
  }
  else printf("test 3 passed!\n");
  /*DEBUG*/
  lseek(file_fd, 0, SEEK_SET);
  char thischar;
  for (i=0; i<45*BLOCK_SIZE; ++i) read(file_fd, &thischar, 1);
  if (thischar == EOF) printf("FOUND IT! -1 at index %d\n", i);
  /*debug*/
  pyramid_close(file_fd, remainder_pyramid);
  free(readbuf);
  free(writebuf);
  
}
