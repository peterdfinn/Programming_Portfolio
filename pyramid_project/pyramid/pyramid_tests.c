#include <stdio.h>

#include "pyramid.h"

//do all functions return what you expect under normal conditions?
//do all functions return what you expect under all error conditions?

int main() {
  pyramid *top = pyramid_init(NULL);
  if (how_many_ids(NULL) == 0) {
    printf("Test 1 passed!\n");
  }
  else {
    printf("Test 1 failed!\n");
    return 0;
  }
  int i;
  pyramid *temp = top;
  for (i = 1; i < 6; ++i) {
    printf("Level %u can store %lu block ids\n", temp->level, how_many_ids(temp));
    if (i != 5) temp = pyramid_init(temp);
  }
  int add_results = 1;
  pyramid *temp_parent;
  for (temp = top; temp != NULL; temp_parent = temp, temp = temp->next_level) {
    for (i = 0; i < how_many_ids(temp); ++i) {
      if (temp == top)
	add_results = add_results && add_id_pyramid((long) i, temp);
      else
	add_results = add_results && add_id_pyramid((long) (i + how_many_ids(temp_parent)), temp);
    }
  }
  if (add_results) printf("Test 3 passed!\n");
  else {
    printf("Test 3 failed!\n");
    return 0;
  }
  add_results = add_results && !add_id_pyramid(2L, NULL);
  add_results = add_results && !add_id_pyramid(-2L, top);
  add_results = add_results && !add_id_pyramid(1L, top);
  add_results = add_results && !add_id_pyramid(4L, top->next_level);
  if (add_results) printf("Test 4 passed!\n");
  else {
    printf("Test 4 failed!\n");
    return 0;
  }
  for (temp = top; temp != NULL; temp = temp->next_level) {
    for (i = 0; i < 4; ++i) {
      printf("i: %d - randomly selected %ld\n", i, randomly_select_id_pyramid(temp));
    }
  }
  return 0;
}
