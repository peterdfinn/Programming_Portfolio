#include <stdio.h>

#include "pyramid.h"

int main() {
  pyramid* pyra = pyramid_init(NULL);
  long new_id;
  int add_return = 1;
  for (new_id = 84L; new_id > 0L; --new_id) {
    add_return = add_return && add_id_pyramid(new_id, pyra);
  }
  if (add_return) printf("Test 1 passed!\n");
  if (!add_return) {
    printf("Test 1 failed!\n");
    pyramid_free(pyra);
    return 0;
  }

  pyramid* curr_level;
  for (curr_level = pyra; curr_level; curr_level = curr_level->next_level) {
    int i;
    for (i = 0; i < how_many_ids(curr_level); ++i) {
      printf("%2ld ", curr_level->identifiers[i]);
    }
    printf("\n");
  }
  printf("\n");

  move_id_in_pyramid(pyra->next_level->next_level, pyra, 50L);

  for (curr_level = pyra; curr_level; curr_level = curr_level->next_level) {
    int i;
    for (i = 0; i < how_many_ids(curr_level); ++i) {
      printf("%2ld ", curr_level->identifiers[i]);
    }
    printf("\n");
  }
  printf("\n");
  
  pyramid_free(pyra);
  return 0;
}
