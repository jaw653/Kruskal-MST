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

typedef struct VERTEX VERTEX;
struct VERTEX {
  int value;
  int index;
};

typedef struct EDGE EDGE;
struct EDGE {
  int weight;
  int vertex_A;
  int vertex_B;
  int index;
};

/* Edge functions */
static EDGE *newEDGE(int, int, int);
static int getVertex(EDGE *, int);
static void setVertex(EDGE *, int , int);
static int getWeight(EDGE *);
static void setWeight(EDGE *, int);
static int getIndex(EDGE *);
static int setIndex(EDGE *, int);

/* Utility Functions */
static void readInFile(FILE *, DA *);

/* Quicksort functions */
static void swap(DA *, int, int);
static int partition(DA *, int, int);
static void quickSort(DA *, int, int);

/* Kruskal functions */
static DA *kruskal(DA *);
static void displayMST(DA *);

int main(int argc, char *argv[]) {
  if (argc == 1)
    return 0;

  int i;
  for (i = 0; i < argc; i++) {
    if (argv[i] == "-v") {
      printf("Jacob A. Wachs\n");
      return 0;
    }
  }

  FILE *graphFile = fopen(argv[1], "r");

  DA *arr = newDA(displayINTEGER);
  readInFile(graphFile, arr);       // Read in the file of edges

  DA *MST = kruskal(arr);






  fclose(graphFile);

  return 0;
}




/******************************************************************************/
/***                           Helper Functions                             ***/
/******************************************************************************/
static EDGE *newEDGE(int a, int b, int weight, int index) {
  EDGE *edge = malloc(sizeof(struct EDGE));
  edge->weight = weight;
  edge->vertex_A = a;
  edge->vertex_B = b;
  edge->index = index;

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

static int getIndex(EDGE *edge) {
  if (edge != NULL)
    return edge->index;
}

static void setIndex(EDGE *edge, int index) {
  if (edge == NULL)
    return;

  edge->index = index;
}

static void readInFile(FILE *fp, DA *array) {
  char *str = readToken(fp);

  while (str) {
    int a = atoi(str);
    int b = atoi(readToken(fp));
    int weight = 1;

    char *final = readToken(fp);

    if (final[0] != ';') {
      weight = atoi(final);
      str = readToken(fp);
    }

    EDGE *edgeToInsert = newEDGE(a, b, weight, sizeDA(array));

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
  int edge1_index = getIndex(edge1);

  int edge2_vertA = getVertex(edge2, 0);
  int edge2_vertB = getVertex(edge2, 1);
  int edge2_weight = getWeight(edge2);
  int edge2_index = getIndex(edge2);

  EDGE *tmp = newEDGE(edge1_vertA, edge1_vertB, edge1_weight, edge1_index);

  setVertex(edge1, 0, edge2_vertA);
  setVertex(edge1, 1, edge2_vertB);
  setWeight(edge1, edge2_weight);
  setIndex(edge1, edge2_index);

  setVertex(edge2, 0, tmp->vertex_A);
  setVertex(edge2, 1, tmp->vertex_B);
  setWeight(edge2, tmp->weight);
  setIndex(edge2, tmp->index);
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

static void quickSort(DA *arr, int low, int high) {
  if (low < high) {
    int pivotLocation = partition(arr, low, high);
    quickSort(arr, low, pivotLocation-1);
    quickSort(arr, pivotLocation+1, high);
  }
}

static DA *kruskal(DA *arr) {
  DA *A = newDA(displayINTEGER);

  int size = sizeDA(arr);
  SET *set = newSET(displayINTEGER);

  // Make-Set for every vertex in the graph
  int i;
  for (i = 0; i < size; i++) {
    int currA = getVertex(getDA(arr, i), 0);
    int currB = getVertex(getDA(arr, i), 1);

    int in1 = makeSET(set, newINTEGER(currA));
    int in2 = makeSET(set, newINTEGER(currB));
  }

  // Sort edges of graph in ascending order by weight
  quickSort(arr, 0, size);        // ?? qsort(arr, size, sizeof(struct EDGE), ...

  for (i = 0; i < size; i++) {
    if (findSET(set, i) != findSET(set, ++i)) {
      insertDA(A, getDA(arr, i-1));
      unionSET(set, i-1, i);
    }
  }

  return A;
}

static void printEDGE(DA *arr, void *edge) {
  EDGE *e = edge;
  int parentVal = getDA(findSET(e->))
  printf(" %d(%d)%d", )

}

static void displayMST(DA *arr) {
  int i;
  int size = sizeDA(arr);

  for (i = 0; i < size; i++) {
    printf("%d:", i);
    printEDGE(arr, getDA(arr, i));
    printf("\n");
  }
}
