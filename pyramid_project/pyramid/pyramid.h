#include <stdlib.h>
#include <unistd.h>

#define KEY_LENGTH 16
#define SIGLEN (384 / 8)
#define BLOCK_SIZE (1 << 12)

typedef struct pyramid_struct {
  unsigned level;
  int pyramid_fd;
  long *identifiers;
  struct pyramid_struct *next_level;
  unsigned char *key;
} pyramid;

size_t how_many_ids(pyramid *p);
int pyramid_overflow(pyramid *p);
int move_id_in_pyramid(pyramid* this_level, pyramid* top_level, long id);
void pyramid_free(pyramid* p);
pyramid* pyramid_init(pyramid* parent, int pyramid_fd, unsigned char *key);
long randomly_select_id_pyramid(pyramid *p);
int add_id_pyramid(long id, pyramid *p);
int encrypt_pyramid(long *from, long *to, pyramid* pyra, int pyramid_size, unsigned char *nonce, long seed);
int verify_pyramid(unsigned char *signature, unsigned char *payload, ssize_t payload_size);
pyramid* pyramid_open(char *pathname, int flags, int* file_fd, int* pyramid_fd, int *err, unsigned char *key);
int pyramid_close(int fd, pyramid* pyra);
void determine_blocks(size_t count, long *first, long *last, off_t position);
void test1();
void test2();
void closeit(int file_fd, pyramid* p, int i);
void print_ids(pyramid* p, int test);
pyramid* openit(char *filename, int* p2fd, unsigned char *key, int i);
void create_big_file(char *filename);
void test3();
ssize_t read_block(int file_fd, long block, pyramid *pyra, long *block_ids,
                    unsigned char *big_buffer);
unsigned how_many_levels(pyramid *pyra);
ssize_t pyramid_read(int fd, unsigned char *buf, size_t count, pyramid *pyra);
ssize_t pyramid_write(int fd, unsigned char *buf, size_t count, pyramid *pyra);
