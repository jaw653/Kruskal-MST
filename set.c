/* Author: Jake Wachs
 * Date: 11/24/2017
 *
 * Implementation file for disjoint set header function
 */

#include <stdio.h>
#include <stdlib.h>

#include "set.h"
#include "da.h"
#include "integer.h"

typedef struct NODE NODE;

struct set {
  DA *list;
  int numReps;
  void (*display)(FILE *, void *);
};

struct NODE {
  void *value;
  int index;
  struct NODE *parent;
};

SET *newSET(void (*d)(FILE *, void *)) {
  SET *set = malloc(sizeof(struct set));
  set->list = newDA(d);
  set->numReps = 0;
  set->display = d;

  return set;
}

int makeSET(SET *d, void *value) {
  insertDA(d->list, value);
  d->numReps += 1;

  return sizeDA(d->list);
}

int findSET(SET *d, int index) {
  //returns the index of the representative of the value at the given index
  NODE *foundNode = getDA(d->list, index);
  NODE *parent = foundNode->parent;

  if (getINTEGER(foundNode->value) == getINTEGER(parent->value))
    return index;

  return findSET(d, getINTEGER(parent->value));
}

int unionSET(SET *d, int index1, int index2) {
  int rep1 = findSET(d, index1);
  int rep2 = findSET(d, index2);

  if (rep1 == rep2)
    return 0;

  NODE *node1 = getDA(d->list, index1);
  NODE *node2 = getDA(d->list, index2);

  node1->parent = node2;

  d->numReps -= 1;
  return 1;
}

int countSET(SET *d) {
  return d->numReps;
}

int displaySET(FILE *fp, SET *d) {
  
}
