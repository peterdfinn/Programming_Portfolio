#include <math.h>
#include <stdlib.h>
#include <strings.h>

#include "pyramid.h"

/* Function: pyramid_init
 *
 * Description: This function initializes and returns a new pyramid struct that
 *  will be the child of the input pyramid struct *parent.
 *
 * Inputs:
 *  parent - if NULL, the created pyramid will be assumed to be the head.
 *   Otherwise, the created pyramid will be added as the child of parent.
 *  pyramid_fd - The file descriptor of the pyramid file.
 *  key - A buffer containing the key to be used for encrypting and decrypting
 *   the pyramid file.
 *
 * Return value:
 *  NULL if there was an error allocating heap space for the new pyramid or if
 *   key is NULL.
 *  Otherwise, a pointer to the new pyramid is returned.
 */
pyramid* pyramid_init(pyramid* parent, int pyramid_fd, unsigned char *key) {
  if (!key) return NULL;
  pyramid *p = malloc(sizeof(pyramid));
  if (!p) return NULL;
  if (!parent) {
    p->level = 1;
    p->pyramid_fd = pyramid_fd;
    p->key = malloc(KEY_LENGTH);
    bcopy(key, p->key, KEY_LENGTH);
  }
  else {
    p->level = parent->level + 1;
    p->pyramid_fd = parent->pyramid_fd;
    p->key = parent->key;
  }
  p->identifiers = NULL;
  p->next_level = NULL;
  size_t howmanyentries = how_many_ids(p);
  if (!howmanyentries) {
    free(p);
    return NULL;
  }
  p->identifiers = calloc(howmanyentries, sizeof(long));
  if (!p->identifiers) {
    free(p);
    return NULL;
  }
  int i;
  for (i = 0; i < howmanyentries; ++i) p->identifiers[i] = -1L;
  if (parent) parent->next_level = p;
  return p;
}
