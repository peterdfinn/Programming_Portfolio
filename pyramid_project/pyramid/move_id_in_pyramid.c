#include "pyramid.h"

/*
 * Function: move_id_in_pyramid
 *
 * Description: This function removes a block identifier from a given level in
 *  the pyramid, if present, and stores it in the top level of the pyramid.
 *
 * Inputs:
 *  this_level - A pointer to the level in the overall pyramid from which to
 *   remove the block identifier.
 *  top_level - A pointer to level 1 of the pyramid.
 *  id - The identifier to move.
 *
 * Return value:
 *  1 if the block identifier was moved successfully.
 *  -1 if any of the arguments is invalid.
 *  -2 if the pyramid *this_level has a NULL identifier list.
 *  -3 if the given block identifier was not found at the specified level of the
 *   pyramid.
 *  -4 if the call to pyramid_overflow failed.
 */
int move_id_in_pyramid(pyramid* this_level, pyramid* top_level, long id) {
  if (!this_level || !top_level || (id < 0)) return -1;
  long* current_ids = this_level->identifiers;
  if (!current_ids) return -2;
  size_t howmanyids = how_many_ids(this_level);
  int i, foundit = 0;
  for (i = 0; i < howmanyids; ++i) {
    int thisisit = (current_ids[i] == id);
    foundit = foundit || thisisit;
    if (thisisit) current_ids[i] = -1L;
  }
  if (!foundit) return -3;
  for (i = 0; i < how_many_ids(top_level); ++i) {
    if (top_level->identifiers[i] < 0L) {
      top_level->identifiers[i] = id;
      break;
    }
  }
  if (i >= how_many_ids(top_level)) {
    if (pyramid_overflow(top_level) < 0) {
      return -4;
    }
    add_id_pyramid(id, top_level);
  }
  return 1;
}
