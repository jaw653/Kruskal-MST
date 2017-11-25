/* Author: Jake Wachs
 * Date: 11/24/2017
 *
 * The University of Alabama
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "scanner.h"
#include "integer.h"
#include "da.h"

typedef struct EDGE EDGE;
struct EDGE {
  int weight;
  int vertex_A;
  int vertex_B;
};

/* Edge functions */
static EDGE *newEDGE(int, int, int);
static int getVertex(EDGE *, int);
static void setVertex(EDGE *, int , int);
static int getWeight(EDGE *);
static void setWeight(EDGE *, int);

/* Utility Functions */
static void readInFile(FILE *, DA *);

/* Quicksort functions */
static void swap(DA *, int, int);
static int partition(DA *, int, int);
static void quickSort(DA *, int, int);

int main(int argc, char *argv[]) {
  FILE *graphFile = fopen(argv[1], "r");

  DA *arr = newDA(displayINTEGER);
  readInFile(graphFile, arr);       // Read in the file of edges

  // First thing to do is sort the edges by weight


  fclose(graphFile);
  return 0;
}




/******************************************************************************/
/***                           Helper Functions                             ***/
/******************************************************************************/

static EDGE *newEDGE(int a, int b, int weight) {
  EDGE *edge = malloc(sizeof(struct EDGE));
  edge->weight = weight;
  edge->vertex_A = a;
  edge->vertex_B = b;

  return edge;
}

static int getVertex(EDGE *edge, int vertex) {
  if (edge == NULL)
    return 0;

  if (vertex == 0)
    return edge->vertex_A;
  else
    return edge->vertex_B;
}

static void setVertex(EDGE *edge, int vertex, int newVal) {
  if (edge == NULL)
    return;

  if (vertex == 0)
    edge->vertex_A = newVal;
  else
    edge->vertex_B = newVal;
}

static int getWeight(EDGE *edge) {
  if (edge == NULL)
    return 0;

  return edge->weight;
}

static void setWeight(EDGE *edge, int weight) {
  if (edge == NULL)
    return;

  edge->weight = weight;
}

static void readInFile(FILE *fp, DA *array) {
  char *str = readToken(fp);

  while (str) {
    int a = atoi(str);
    int b = atoi(readToken(fp));
    int weight = 1;

    char *final = readToken(fp);

    if (strcmp(final, ";") != 0)
      weight = atoi(final);

    EDGE *edgeToInsert = newEDGE(a, b, weight);

    insertDA(array, edgeToInsert);

    str = readToken(fp);
  }
}

static void swap(DA *arr, int index1, int index2) {
  EDGE *edge1 = getDA(arr, index1);
  EDGE *edge2 = getDA(arr, index2);

  int edge1_vertA = getVertex(edge1, 0);
  int edge1_vertB = getVertex(edge1, 1);
  int edge1_weight = getWeight(edge1);

  int edge2_vertA = getVertex(edge2, 0);
  int edge2_vertB = getVertex(edge2, 1);
  int edge2_weight = getWeight(edge2);

  EDGE *tmp = newEDGE(edge1_vertA, edge1_vertB, edge1_weight);

  setVertex(edge1, 0, edge2_vertA);
  setVertex(edge1, 1, edge2_vertB);
  setWeight(edge1, edge2_weight);

  setVertex(edge2, 0, tmp->vertex_A);
  setVertex(edge2, 1, tmp->vertex_B);
  setWeight(edge2, tmp->weight);
}

static int partition(DA *arr, int low, int high) {
  EDGE *pivot = getDA(arr, low);
  int leftWall = low;

  int i;
  for (i = low + 1; i < high; i++) {
    // if a[i] < pivot
    EDGE *edge_i = getDA(arr, i);
    if (getWeight(edge_i) < getWeight(pivot)) {
      leftWall += 1;
      swap(arr, i, leftWall);
    }
  }

  swap(arr, low, leftWall);

  return ++leftWall;
}
