#include <stdlib.h>

#include "pyramid.h"

/* Function: pyramid_free
 *
 * This function frees heap-allocated memory associated with a pyramid struct,
 * including that of its descendants, and including the pyramid pointer itself.
 */
void pyramid_free(pyramid* p) {
  if (!p) return;
  if (p->next_level != NULL) pyramid_free(p->next_level);
  if (p->identifiers != NULL) free(p->identifiers);
  if ((p->key != NULL) && (p->next_level == NULL)) free(p->key);
  free(p);
  return;
}
