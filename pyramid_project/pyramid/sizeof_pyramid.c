#include "pyramid.h"

/*
 * Function: sizeof_pyramid
 *
 * Description: This function returns the number of identifiers in the full
 *  pyramid (not just this level) for the purposes of writing the full
 *  pyramid to a file.
 *
 * Inputs:
 *  pyra: A pointer to the top level of the pyramid.
 *
 * Return value:
 *  0 in the case of an error (i.e., pyra is NULL or isn't the top level of its
 *   pyramid).
 *  Otherwise, this function returns the number of block identifiers that may
 *   be stored in the entire pyramid.
 */
size_t sizeof_pyramid(pyramid* pyra) {
  if (!pyra) return 0;
  if (pyra->level != 1) return 0;
  
  pyramid* p = pyra;
  size_t total = 0;
  while (p->next_level) {
    total += how_many_ids(p);
    p = p->next_level;
  }
  total += how_many_ids(p);

  return total;
}
