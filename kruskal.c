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
#include "bst.h"
#include "rbt.h"
#include "queue.h"
#include "cda.h"

typedef struct EDGE EDGE;
struct EDGE {
  int u, v, weight;
  int u_index, v_index;
};

/* Edge functions */
static EDGE *newEDGE(int, int, int, int, int);
static void displayEDGE(FILE *, void *);

/* Utility Functions */
static void readInFile(FILE *, DA *, DA *, RBT *);

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
  RBT *vertexTree = newRBT(displayINTEGER, compareINTEGER);

  readInFile(graphFile, edgeArr, vertexArr, vertexTree);

  fclose(graphFile);
//  qsort(vertexArr, sizeDA(vertexArr), sizeof(struct VERTEX), compareVERTEX);

printf("edgeArr is: \n");
displayDA(stdout, edgeArr);
printf("\n");
printf("vertexArr is: \n");
displayDA(stdout, vertexArr);
printf("\n");

  DA *MST = kruskal(edgeArr, vertexArr);
  return 0;
}




/******************************************************************************/
/***                           Helper Functions                             ***/
/******************************************************************************/
static EDGE *newEDGE(int u, int v, int weight, int index1, int index2) {
  EDGE *e = malloc(sizeof(struct EDGE));
  e->u = u;
  e->v = v;
  e->weight = weight;
  e->u_index = index1;
  e->v_index = index2;

  return e;
}

static void readInFile(FILE *fp, DA *edgeArr, DA *vertexArr, RBT *tree) {
  char *str = readToken(fp);

  int vertexIndex = 0;
  while (str) {
    int u = atoi(str);
    int v = atoi(readToken(fp));
    int weight = 1;

    char *final = readToken(fp);

    if (final[0] != ';') {
      weight = atoi(final);
      str = readToken(fp);
    }

    printf("u index is %d and v index is %d\n", vertexIndex, vertexIndex+1);
    EDGE *edgeToInsert = newEDGE(u, v, weight, vertexIndex, vertexIndex+1);
    vertexIndex += 2;


    if (findRBT(tree, newINTEGER(u)) == 0) {
      insertRBT(tree, newINTEGER(u));
      insertDA(vertexArr, newINTEGER(u));
    }
    else {
      //edgeToInsert->u_index = whatever the index of the first one inserted was
      // idea:
      // sort the array of vertices. binary search it for the one with the same value as the vertex to be insertedNode
      // set edgeToInsert->u_index = the index of the result of the binary search
    }

    if (findRBT(tree, newINTEGER(v)) == 0) {
      insertRBT(tree, newINTEGER(v));
      insertDA(vertexArr, newINTEGER(v));
    }

    insertDA(edgeArr, edgeToInsert);


    str = readToken(fp);
  }
}

static void displayEDGE(FILE *fp, void *edge) {
  EDGE *e = edge;
  fprintf(fp, "%d->%d(%d)", e->u, e->v, e->weight);
}

static void swap(DA *arr, int index1, int index2) {
  EDGE *edge1 = getDA(arr, index1);
  EDGE *edge2 = getDA(arr, index2);

  int u1 = edge1->u;
  int v1 = edge1->v;
  int weight1 = edge1->weight;
  int udex1 = edge1->u_index;
  int vdex1 = edge1->v_index;

  int u2 = edge2->u;
  int v2 = edge2->v;
  int weight2 = edge2->weight;
  int udex2 = edge2->u_index;
  int vdex2 = edge2->v_index;

  EDGE *temp = newEDGE(u1, v1, weight1, udex1, vdex1);

  edge1->u = u2;
  edge1->v = v2;
  edge1->weight = weight2;
  edge1->u_index = udex2;
  edge1->v_index = vdex2;

  edge2->u = temp->u;
  edge2->v = temp->v;
  edge2->weight = temp->weight;
  edge2->u_index = temp->u_index;
  edge2->v_index = temp->v_index;
}

static int partition(DA *arr, int low, int high) {
  EDGE *pivot = getDA(arr, low);
  int leftWall = low;

  int i;
  for (i = low + 1; i < high; i++) {
    // if a[i] < pivot
    EDGE *edge_i = getDA(arr, i);
    if (edge_i->weight < pivot->weight) {
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

static DA *kruskal(DA *edgeArr, DA *vertexArr) {
  DA *A = newDA(displayINTEGER);

  int numVertices = sizeDA(vertexArr);
  int numEdges = sizeDA(edgeArr);
  SET *set = newSET(displayINTEGER);

  /* Making set for all vertices */
  int i;
  for (i = 0; i < numVertices; i++) {
    INTEGER *intToInsert = getDA(vertexArr, i);
    makeSET(set, intToInsert);
  }

  // sort the edges by weight
  quickSort(edgeArr, 0, numEdges);
printf("sorted edgeArr: \n");
displayDA(stdout, edgeArr);
printf("\n");

  for (i = 0; i < numEdges; i++) {
    EDGE *currEdge = getDA(edgeArr, i);
    int Udex = currEdge->u_index;
    int Vdex = currEdge->v_index;
    printf("%d->%d(%d) || uIndex = %d, vIndex = %d\n", currEdge->u, currEdge->v, currEdge->weight, Udex, Vdex);
    if (findSET(set, Udex) != findSET(set, Vdex)) {
      insertDA(A, currEdge);
      unionSET(set, Udex, Vdex);
    }
  }

  return A;
}
