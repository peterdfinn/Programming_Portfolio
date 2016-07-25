#include <stdio.h>

#include "pyramid.h"

/*
 * Return value:
 *  1 upon success.
 *  -1 if p is NULL.
 *  -2 if a call to pyramid_init failed.
 *  -3 if a call to add_id_pyramid failed.
 */
int pyramid_overflow(pyramid *p) {
  if (!p) return -1;
  if (!p->next_level) {
    if (!pyramid_init(p, p->pyramid_fd, p->key)) return -2;
  }
  
  int idx;
  for (idx = 0; idx < how_many_ids(p); ++idx) {
    if (!add_id_pyramid(p->identifiers[idx], p->next_level))
      return -3;
    p->identifiers[idx] = -1L;
  }
  return 1;
}
