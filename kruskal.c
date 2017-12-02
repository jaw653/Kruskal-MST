/* Author: Jake Wachs
 * Date: 11/24/2017
 *
 * The University of Alabama
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
  VERTEX *u;
  VERTEX *v;
};

/* Vertex functions */
static VERTEX *newVERTEX(int, int);

/* Edge functions */
static EDGE *newEDGE(VERTEX *, VERTEX *, int);
static VERTEX *getVertex(EDGE *, int);
static void setVertex(EDGE *, int , VERTEX *);
//static int getWeight(EDGE *);
//static void setWeight(EDGE *, int);
static void displayEDGE(FILE *, void *);

/* Utility Functions */
static void readInFile(FILE *, DA *, DA *);
static DA *removeDuplicates(DA *, int);

/* Quicksort functions */
static void swap(DA *, int, int);
static int partition(DA *, int, int);
static void quickSort(DA *, int, int);

/* Kruskal functions */
static DA *kruskal(DA *, DA *);
//static void displayMST(DA *);

int main(int argc, char *argv[]) {
  if (argc == 1)
    return 0;

  int i;
  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0) {
      printf("Jacob A. Wachs\n");
      return 0;
    }
  }

  FILE *graphFile = fopen(argv[1], "r");

  DA *edgeArr = newDA(displayEDGE);
  DA *vertexArr = newDA(displayINTEGER);

  readInFile(graphFile, edgeArr, vertexArr);       // Read in the file of edges

  vertexArr = removeDuplicates(vertexArr, sizeDA(vertexArr));
  printf("vertexArr is: \n");
  displayDA(stdout, vertexArr);
  printf("\n");

  fclose(graphFile);

  DA *MST = kruskal(edgeArr, vertexArr);

  displayDA(stdout, MST);
  printf("\n");

  return 0;
}




/******************************************************************************/
/***                           Helper Functions                             ***/
/******************************************************************************/
static VERTEX *newVERTEX(int value, int index) {
  VERTEX *v = malloc(sizeof(struct VERTEX));
  v->value = value;
  v->index = index;

  return v;
}

static EDGE *newEDGE(VERTEX *u, VERTEX *v, int weight) {
  EDGE *edge = malloc(sizeof(struct EDGE));
  edge->u = u;
  edge->v = v;
  edge->weight = weight;

  return edge;
}


static VERTEX *getVertex(EDGE *edge, int vertex) {
  if (edge == NULL)
    return 0;

  if (vertex == 0)
    return edge->u;
  else
    return edge->v;
}

static void setVertex(EDGE *edge, int vertex, VERTEX *v) {
  if (edge == NULL)
    return;

  if (vertex == 0)
    edge->u = v;
  else
    edge->v = v;
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

static void readInFile(FILE *fp, DA *array, DA *vertexArr) {
  char *str = readToken(fp);
  int index = 0;

  while (str) {
    int a = atoi(str);
    int b = atoi(readToken(fp));
    int weight = 1;

    char *final = readToken(fp);

    if (final[0] != ';') {
      weight = atoi(final);
      str = readToken(fp);
    }

    VERTEX *u = newVERTEX(a, index);
    insertDA(vertexArr, u);
    VERTEX *v = newVERTEX(b, ++index);
    insertDA(vertexArr, v);
    index += 1;

    EDGE *edgeToInsert = newEDGE(u, v, weight);

    insertDA(array, edgeToInsert);

    str = readToken(fp);
  }
}

static DA *removeDuplicates(DA *vertexArr, int size) {
  // sort array
  if (size == 0 || size == 1)
    return vertexArr;

  DA *temp = newDA(displayINTEGER);

  int i;
  for (i = 0; i < size-1; i++) {
    VERTEX *vi = getDA(vertexArr, i);
    VERTEX *vii = getDA(vertexArr, i+1);
    printf("vi value is: %d and vii value is: %d\n", vi->value, vii->value);
    if (vi->value != vii->value) {
      printf("adding %d to temp\n", vi->value);
      insertDA(temp, vi);
    }
  }

  insertDA(temp, getDA(vertexArr, size-1));

  return temp;
}


static void swap(DA *arr, int index1, int index2) {
  EDGE *edge1 = getDA(arr, index1);
  EDGE *edge2 = getDA(arr, index2);

  VERTEX *edge1_vertA = getVertex(edge1, 0);
  VERTEX *edge1_vertB = getVertex(edge1, 1);
  int edge1_weight = getWeight(edge1);

  VERTEX *edge2_vertA = getVertex(edge2, 0);
  VERTEX *edge2_vertB = getVertex(edge2, 1);
  int edge2_weight = getWeight(edge2);

  EDGE *tmp = newEDGE(edge1_vertA, edge1_vertB, edge1_weight);

  setVertex(edge1, 0, edge2_vertA);
  setVertex(edge1, 1, edge2_vertB);
  setWeight(edge1, edge2_weight);

  setVertex(edge2, 0, tmp->u);
  setVertex(edge2, 1, tmp->v);
  setWeight(edge2, tmp->weight);
}

static int partitionV(DA *arr, int low, int high) {
  VERTEX *pivot = getDA(arr, low);
  int leftWall = low;

  int i;
  for (i = low + 1; i < high; i++) {
    VERTEX *vertex_i = getDA(arr, i);
    if (vertex_i->value < pivot->value) {
      leftWall += 1;

    }
  }
}

static int partitionE(DA *arr, int low, int high) {
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

static void quickSort(DA *arr, int low, int high, char eOrV) {
  if (low < high) {
    int pivotLocation = 0;
    if (eOrV == 'e')
      pivotLocation = partitionE(arr, low, high);
    else
      pivotLocation = partitionV(arr, low, high);
    quickSort(arr, low, pivotLocation-1);
    quickSort(arr, pivotLocation+1, high);
  }
}

static DA *kruskal(DA *edgeArr, DA *vertexArr) {
  DA *A = newDA(displayEDGE);

  int size = sizeDA(edgeArr);
  int numVertices = sizeDA(vertexArr);
  SET *set = newSET(displayINTEGER);
  //DA *vertexArr = newDA(NULL);

  // Make-Set for every vertex in the graph
  int i;
  for (i = 0; i < numVertices; i++) {              // If vertex already exists, don't add it
    VERTEX *u = getDA(vertexArr, i);
    makeSET(set, newINTEGER(u->value));

    VERTEX *v = getDA(vertexArr, i);
    makeSET(set, newINTEGER(v->value));
  }

  // Sort edges of graph in ascending order by weight
  quickSort(edgeArr, 0, size);        // ?? qsort(arr, size, sizeof(struct EDGE), ...

  int index = 0;
  for (i = 0; i < size; i++) {
    EDGE *currEdge = getDA(edgeArr, i);
    VERTEX *u = getVertex(currEdge, 0);
    VERTEX *v = getVertex(currEdge, 1);
    int uIndex = u->index;
    int vIndex = v->index;

    if (findSET(set, uIndex) != findSET(set, vIndex)) {
      insertDA(A, getDA(edgeArr, index++));
      unionSET(set, uIndex, vIndex);
    }
  }

  return A;
}

static void displayEDGE(FILE *fp, void *edge) {
  EDGE *e = edge;
  fprintf(fp, "%d-%d(%d)", getVertex(e, 0)->value, getVertex(e, 1)->value, e->weight);
}

/*
static bool edgeExists(DA *edgeArr, EDGE *e) {
  int i;
  int size = sizeDA(edgeArr);

  for (i = 0; i < size; i++) {
    if ()
  }
}
*/

/*
static void printEDGE(DA *arr, void *edge) {
  EDGE *e = edge;
  //int parentVal = getDA(findSET(e->))
  printf(" 1" );

}
*/
/*
static void displayMST(DA *arr) {
  int i;
  int size = sizeDA(arr);

  for (i = 0; i < size; i++) {
    printf("%d:", i);
    printEDGE(arr, getDA(arr, i));
    printf("\n");
  }
}
*/
