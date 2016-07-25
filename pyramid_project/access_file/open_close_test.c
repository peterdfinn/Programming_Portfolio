#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../pyramid/pyramid.h"


void test3() {
  int i;
  unsigned char key[KEY_LENGTH];
  for (i = 0; i < KEY_LENGTH; ++i) key[i] = (unsigned char) (rand() % 256);
  create_big_file("test3file");
  for (i = 1; i <= 3; ++i) {
    int p3fd;
    pyramid *p = openit("test3file", &p3fd, key, i);
    print_ids(p, 3);
    move_id_in_pyramid(p, p, randomly_select_id_pyramid(p));
    move_id_in_pyramid(p->next_level, p, randomly_select_id_pyramid(p->next_level));
    move_id_in_pyramid(p->next_level->next_level, p, randomly_select_id_pyramid(p->next_level->next_level));
    print_ids(p, 3);
    closeit(p3fd, p, i);
  }
}

void test2() {
  int i;
  create_big_file("test2file");
  unsigned char key[KEY_LENGTH];
  for (i = 0; i < KEY_LENGTH; ++i) key[i] = (unsigned char) (110 - i);

  for (i = 1; i <= 3; ++i) {
    int p2fd;
    pyramid* p = openit("test2file", &p2fd, key, i);
    print_ids(p, 2);
    closeit(p2fd, p, i);
  }
  
}

void closeit(int file_fd, pyramid* p, int i) {
  int closeret = pyramid_close(file_fd, p);
  if (closeret) {
    printf("test2: close %d failed; returned %d\n", i, closeret);
  }
  pyramid_free(p);
}

void print_ids(pyramid* p, int test) {
  printf("For test %d, block ids are:\n", test);
  int i;
  pyramid* this = p;
  while (this) {
    for (i = 0; i < how_many_ids(this); ++i) printf("%ld ", this->identifiers[i]);
    putchar(10);
    this = this->next_level;
  }
  return;
}

pyramid* openit(char *filename, int* p2fd, unsigned char *key, int i) {
  int p2pfd;
  int err2;
  pyramid* retval = pyramid_open(filename, O_RDWR, p2fd, &p2pfd, &err2, key);
  if (!retval) {
    printf("test2: open %d failed; err=%d\n", i, err2);
  }
  return retval;
}

void create_big_file(char *filename) {
  size_t sizeoffile = 23 * BLOCK_SIZE;
  unsigned char *buf = malloc(sizeoffile);
  size_t i;
  for (i = 0; i < sizeoffile; ++i) buf[i] = (unsigned char) ((i + 20) % 251);
  int fd = open(filename, O_CREAT|O_RDWR|O_TRUNC);
  fchmod(fd, S_IRUSR|S_IWUSR);
  write(fd, buf, sizeoffile);
  close(fd);
  free(buf);
  return;
}

void test1() {
  int p1fd, p1pfd, err1;

  unsigned char key[KEY_LENGTH];
  int i;
  for (i = 0; i < KEY_LENGTH; ++i)
    key[i] = (unsigned char) 10;
  pyramid *p1 = pyramid_open("../access_file/bestoftimes.txt", O_RDWR, &p1fd, &p1pfd, &err1, key);
  if (!p1) {
    printf("pyramid open returned NULL\nerr=%d\n", err1);
    return;
  }

  pyramid *this = p1;
  while (this) {
    for (i = 0; i < how_many_ids(this); ++i) {
      printf("%ld\t", this->identifiers[i]);
    }
    putchar(10);
    this = this->next_level;
  }

  int closeret = pyramid_close(p1fd, p1);
  if (closeret) {
    printf("pyramid close failed; returned %d\n", closeret);
    pyramid_free(p1);
    return;
  }

  printf("test1 passed\n");
  pyramid_free(p1);
  return;
}
