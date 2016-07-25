#include "pyramid.h"

/*
 * Function: how_many_levels
 *
 * Description: This function, when given any pyramid level, returns the
 *  number of levels in the pyramid.
 *
 * Inputs:
 *  pyra - A pointer to any level in the pyramid.
 *
 * Return value:
 *  0 if pyra is NULL.
 *  Otherwise, the number of levels in the full pyramid is returned.
 */
unsigned how_many_levels(pyramid *pyra) {
  if (!pyra) return 0;
  pyramid *p = pyra;
  while (p->next_level) p = p->next_level;
  return p->level;
}
