#include <math.h>

#include "pyramid.h"

/*
 * Function: how_many_ids
 *
 * Description: This function calculates the total number of block identifiers
 *  that may be stored in the input pyramid (or rather, at that level of the
 *  main pyramid).
 *
 * Inputs:
 *  p - A pointer to the pyramid level.
 *
 * Return value:
 *  0 if p is a NULL pointer.
 *  Otherwise, returns 4 to the power of the level of *p.
 */
size_t how_many_ids(pyramid *p) {
  if (!p) return 0;
  unsigned level = p->level;
  double howmany = pow(4.0, (double) level);
  return (size_t) howmany;
}
