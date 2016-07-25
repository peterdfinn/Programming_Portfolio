#include <stdlib.h>

#include "pyramid.h"

/*
 * Function: randomly_select_id_pyramid
 *
 * Description: This function, when passed a pointer to a level of the pyramid,
 *  randomly selects one of the block identifiers stored there.
 *
 * Inputs:
 *  p - A pointer to the pyramid level to search within.
 *
 * Return value:
 *  -1 if a call to how_many_ids failed.
 *  -2 if the block identifier list at this level of the pyramid was empty.
 *  -3 if the input pyramid level is NULL or its list of block identifiers is
 *   NULL.
 *  Otherwise, the randomly selected block identifier is returned.
 */
long randomly_select_id_pyramid(pyramid *p) {
  if (!p || !p->identifiers) return -3L;
  size_t howmanyids = how_many_ids(p);
  if (!howmanyids) return -1L;
  int r = rand() % howmanyids;
  int i = r;
  do {
    if (p->identifiers[i] >= 0) {
      return p->identifiers[i];
    }
    i = (i + 1) % howmanyids;
  } while (i != r);
  return -2L;
}
