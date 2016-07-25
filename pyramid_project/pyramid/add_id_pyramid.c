#include "pyramid.h"

/*
 * Function: add_id_pyramid
 *
 * Description: This function adds a given block identifier to the specified
 *  level in the pyramid, dumping the contents of that level onto the next if
 *  there isn't enough space for it.
 *
 * Inputs:
 *  id - The number of the block to be added.
 *  p - A pointer to the level of the pyramid at which to add the block
 *   identifier.
 *
 * Return value:
 *  0 if p is NULL, id is invalid, or a block with identifier id was already
 *   found at that level in the pyramid.
 *  1 if the block was successfully added.
 */
int add_id_pyramid(long id, pyramid *p) {
  if (!p || (id < 0)) return 0;
  int i;
  int empty = -1;
  
  for (i = 0; i < how_many_ids(p); ++i) {
    if (p->identifiers[i] < 0) {
      empty = i;
    }
    else if (p->identifiers[i] == id) {
      return 0;
    }
  }
  if (empty != -1) {
    p->identifiers[empty] = id;
    return 1;
  }
  pyramid_overflow(p);
  return add_id_pyramid(id, p);
}
