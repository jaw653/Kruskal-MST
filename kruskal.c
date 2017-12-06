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

typedef struct NODE NODE;
struct NODE {
  int value, parent, weight, visited;
  DA *list;
};

typedef struct EDGE EDGE;
struct EDGE {
  int u, v, weight;
  int u_index, v_index;
};

/* MST functions */
static void displayMST(DA *);

/* Node functions */
static NODE *newNODE(int, int, int);
static void displayNODE(NODE *, int);
static void addAdjacentNodes(DA *, DA *);
static int binarySearchNodeIndex(DA *, int, int, int);
static NODE *findMainVersion(DA *, NODE *);

/* Edge functions */
static EDGE *newEDGE(int, int, int, int, int);
static void displayEDGE(FILE *, void *);

/* Utility Functions */
static void readInFile(FILE *, DA *, DA *, RBT *);
static int intCompare(const void *, const void *);
static int *sortVertices(DA *, int);
static int retrieveVertexIndex(int *, int, int, int);

/* QuickSort functions */
static void swap(DA *, int, int);
static int partition(DA *, int, int, char);
static void quickSort(DA *, int, int, char);

/* Kruskal functions */
static DA *kruskal(DA *, int *, int);
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

  //sort the array of vertices
  int numVertices = sizeDA(vertexArr);
  int *primativeVertexArr = sortVertices(vertexArr, numVertices);

  fclose(graphFile);

  DA *MST = kruskal(edgeArr, primativeVertexArr, numVertices);

/*
  printf("after kruskal: \n");
  displayDA(stdout, MST);
  printf("\n");
*/


  //quickSort(MST, 0, sizeDA(MST), 'v');    // FIXME: this might be completely unneeded

  /* Adding ordered list of vertices to be the 'spine' of the adjacency list */
  DA *adjacencyList = newDA(displayINTEGER);    //FIXME: this might need to be displayNODE
  for (i = 0; i < numVertices; i++) {
     NODE *n = newNODE(primativeVertexArr[i], 0, 0);
     insertDA(adjacencyList, n);
   }

  // Adding adjacency lists to each 'spine' value
  addAdjacentNodes(adjacencyList, MST);

  // Sort each individual adjacency list
  for (i = 0; i < numVertices; i++) {
    NODE *n = getDA(adjacencyList, i);
    DA *currDA = n->list;
    quickSort(currDA, 0, sizeDA(currDA), 'v');
//    printf("%d->", n->value);
//    displayDA(stdout, currDA);
//    printf("\n");
  }

    // printing the adjacency lists below to know that they are correct, will ultimately get rid of this...
    printf("printing adjacency lists...\n");
    for (i = 0; i < sizeDA(adjacencyList); i++) {
      NODE *curr = getDA(adjacencyList, i);
      printf("%d->", curr->value);
      displayDA(stdout, curr->list);
      printf("\n");
    }

  displayMST(adjacencyList);
  //printf("\n");

  // sort the MST edges based on vertices to get the smallest vertex at the beginning
  // create an adjacency list for the MST
  // walk the sorted MST. If the edge is not visited, run BFS on it

  return 0;
}



/******************************************************************************/
/***                           MST Functions                                ***/
/******************************************************************************/
static void displayMST(DA *adjacencyList) {
  if (sizeDA(adjacencyList) == 0) {   //FIXME: might just get rid of this...could throw off time complexity or be weird on an edge case
    printf("EMPTY\n");
    exit(0);
  }

  QUEUE *currQueue = newQUEUE(NULL);

  int size = sizeDA(adjacencyList);
  int level = 0;
  enqueue(currQueue, getDA(adjacencyList, 0));
  bool isRoot = true;
  int mainIndex = 1;

  while (sizeQUEUE(currQueue) > 0) {
    QUEUE *nextQueue = newQUEUE(NULL);

    printf("%d :", level++);

    if (isRoot) {
      NODE *currNode = dequeue(currQueue);
      displayNODE(currNode, 1);
      currNode = findMainVersion(adjacencyList, currNode);
      currNode->visited = 1;
      // enqueue every node currNode's adjacencyList to nextQueue
      // in this case, it's only got one
      int sizeAdjList = sizeDA(currNode->list);
      int i;
      for (i = 0; i < sizeAdjList; i++) {
        NODE *currAdjListMem = getDA(currNode->list, i);
        if (findMainVersion(adjacencyList, currAdjListMem)->visited == 0) {
          enqueue(nextQueue, currAdjListMem);
        }
      }

      currQueue = nextQueue;

      isRoot = false;
      printf("\n");
    }
    else {
      int i;
      int sizeCurr = sizeQUEUE(currQueue);
      for (i = 0; i < sizeCurr; i++) {
        NODE *currNode = dequeue(currQueue);
        displayNODE(currNode, 0);
        currNode = findMainVersion(adjacencyList, currNode);
        currNode->visited = 1;

        int sizeAdjList = sizeDA(currNode->list);
        int j;
        for (j = 0; j < sizeAdjList; j++) {
          NODE *currAdjListMem = getDA(currNode->list, j);
          if (findMainVersion(adjacencyList, currAdjListMem)->visited == 0) {
            enqueue(nextQueue, currAdjListMem);
          }
        }
      }

      printf("\n");
      currQueue = nextQueue;

      if (sizeQUEUE(currQueue) == 0) {
        int i;
        for (i = mainIndex; i < size; i++) {
          NODE *currNode = getDA(adjacencyList, i);
          if (currNode->visited == 0) {
            enqueue(currQueue, currNode);
            break;
          }
        }
      }
    }
  }
}

/******************************************************************************/
/***                           Node Functions                               ***/
/******************************************************************************/
static NODE *newNODE(int value, int parent, int weight) {
  NODE *n = malloc(sizeof(struct NODE));
  n->value = value;
  n->parent = parent;
  n->weight = weight;
  n->visited = 0;
  n->list = newDA(displayINTEGER);      //FIXME: might need to make a displayNODE function that goes here

  return n;
}

static void displayNODE(NODE *n, int root) {   // Note that the edge arr is the MST
  if (root == 1)
    printf(" %d", n->value);
  else
    printf(" %d(%d)%d", n->value, n->parent, n->weight);
}

static void addAdjacentNodes(DA *adjacencyList, DA *edgeArr) {
  //add currEdge adjacency info to adjacencyList
  int i;
  int size = sizeDA(edgeArr);
  for (i = 0; i < size; i++) {
    EDGE *currEdge = getDA(edgeArr, i);
    //find node with value u in adjacencyList. add v to u's list
    int uIndex = binarySearchNodeIndex(adjacencyList, 0, size, currEdge->u);
    NODE *uNode = getDA(adjacencyList, uIndex);
    NODE *vInsert = newNODE(currEdge->v, currEdge->u, currEdge->weight);
    insertDA(uNode->list, vInsert);
    //Setting the main version's attributes
    NODE *mainV = findMainVersion(adjacencyList, vInsert);
    mainV->parent = currEdge->u;
    mainV->weight = currEdge->weight;

    //find node with value v adjacencyList. Add u to v's list
    int vIndex = binarySearchNodeIndex(adjacencyList, 0, size, currEdge->v);
    NODE *vNode = getDA(adjacencyList, vIndex);
    NODE *uInsert = newNODE(currEdge->u, currEdge->v, currEdge->weight);
    insertDA(vNode->list, uInsert);

    //Setting the main version's attributes
    NODE *mainU = findMainVersion(adjacencyList, uInsert);
    mainU->parent = currEdge->v;
    mainU->weight = currEdge->weight;

  }
}

static int binarySearchNodeIndex(DA *arr, int low, int high, int value) {
  int middle = low + (high - low)/2;
  NODE *curr = getDA(arr, middle);
  if (value == curr->value)
    return middle;
  else if (value < curr->value)
    return binarySearchNodeIndex(arr, low, middle-1, value);
  else
    return binarySearchNodeIndex(arr, middle+1, high, value);
}

static NODE *findMainVersion(DA *adjList, NODE *n) {
  int size = sizeDA(adjList);
  int adjListIndex = binarySearchNodeIndex(adjList, 0, size, n->value);

  NODE *finalNode = getDA(adjList, adjListIndex);

  return finalNode;
}


/******************************************************************************/
/***                           Edge Functions                               ***/
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

static void displayEDGE(FILE *fp, void *edge) {
  EDGE *e = edge;
  fprintf(fp, "%d->%d(%d)", e->u, e->v, e->weight);
}


/******************************************************************************/
/***                           Utility Functions                            ***/
/******************************************************************************/
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

    EDGE *edgeToInsert = newEDGE(u, v, weight, vertexIndex, vertexIndex+1);
    vertexIndex += 2;


    if (findRBT(tree, newINTEGER(u)) == 0) {
      insertRBT(tree, newINTEGER(u));
      insertDA(vertexArr, newINTEGER(u));
    }

    if (findRBT(tree, newINTEGER(v)) == 0) {
      insertRBT(tree, newINTEGER(v));
      insertDA(vertexArr, newINTEGER(v));
    }

    insertDA(edgeArr, edgeToInsert);


    str = readToken(fp);
  }
}

static int intCompare(const void *a, const void *b) {
  const int *ia = (const int *)a;
  const int *ib = (const int *)b;

  return *ia - *ib;
}

static int *sortVertices(DA *arr, int size) {
  int *A = malloc(sizeof(int) * size);
  /* Read dynamic array into an int array */
  int i;
  for (i = 0; i < size; i++) {
    A[i] = getINTEGER(getDA(arr, i));
  }

  /* Use built-in quickSort Utility to sort the new array */
  qsort(A, size, sizeof(int), intCompare);

  return A;
}

static int retrieveVertexIndex(int *arr, int low, int high, int value) {
  int middle = low + (high - low)/2;
  if (value == arr[middle])
    return middle;
  else if (value < arr[middle])
    return retrieveVertexIndex(arr, low, middle-1, value);
  else
    return retrieveVertexIndex(arr, middle+1, high, value);
}


/******************************************************************************/
/***                           QuickSort Functions                          ***/
/******************************************************************************/
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

static int partition(DA *arr, int low, int high, char e) {
  EDGE *pivot = getDA(arr, low);
  int leftWall = low;

  int i;
  for (i = low + 1; i < high; i++) {
    // if a[i] < pivot
    EDGE *edge_i = getDA(arr, i);
    if (e == 'e') {
      if (edge_i->weight < pivot->weight) {
        leftWall += 1;
        swap(arr, i, leftWall);
      }
    }
    else if (e == 'u') {
      if (edge_i->u < pivot->u) {
        leftWall += 1;
        swap(arr, i, leftWall);
      }
    }
    else if (e == 'v') {
      if (edge_i->v < pivot->v) {
        leftWall += 1;
        swap(arr, i, leftWall);
      }
    }
  }

  swap(arr, low, leftWall);

  return ++leftWall;
}

static void quickSort(DA *arr, int low, int high, char e) {
  if (low < high) {
    if (e == 'e') {
      int pivotLocation = partition(arr, low, high, 'e');
      quickSort(arr, low, pivotLocation-1, 'e');
      quickSort(arr, pivotLocation+1, high, 'e');
    }
    else if (e == 'u'){
      int pivotLocation = partition(arr, low, high, 'u');
      quickSort(arr, low, pivotLocation-1, 'u');
      quickSort(arr, pivotLocation+1, high, 'u');
    }
    else if (e == 'v') {
      int pivotLocation = partition(arr, low, high, 'v');
      quickSort(arr, low, pivotLocation-1, 'v');
      quickSort(arr, pivotLocation+1, high, 'v');
    }
  }
}


/******************************************************************************/
/***                           Kruskal Functions                            ***/
/******************************************************************************/
static DA *kruskal(DA *edgeArr, int *vertexArr, int numVertices) {
  DA *A = newDA(displayEDGE);

  int numEdges = sizeDA(edgeArr);
  SET *set = newSET(displayINTEGER);

  /* Making set for all vertices */
  int i;
  for (i = 0; i < numVertices; i++) {
    makeSET(set, newINTEGER(vertexArr[i]));
  }

  // sort the edges by weight
  quickSort(edgeArr, 0, numEdges, 'e');

  for (i = 0; i < numEdges; i++) {
    EDGE *currEdge = getDA(edgeArr, i);
    int Udex = retrieveVertexIndex(vertexArr, 0, numVertices, currEdge->u);
    int Vdex = retrieveVertexIndex(vertexArr, 0, numVertices, currEdge->v);
    //printf("%d->%d(%d) || uIndex = %d, vIndex = %d\n", currEdge->u, currEdge->v, currEdge->weight, Udex, Vdex);
    if (findSET(set, Udex) != findSET(set, Vdex)) {
      insertDA(A, currEdge);
      unionSET(set, Udex, Vdex);
    }
  }

  return A;
}
