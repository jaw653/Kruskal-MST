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
static void specialDisplayNODE(FILE *, void *);
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
static void swapNodes(DA *, int, int);
static int partition(DA *, int, int, char);
static void quickSort(DA *, int, int, char);
static void sortQueue(QUEUE *);

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
printf("flag0\n");
  DA *MST = kruskal(edgeArr, primativeVertexArr, numVertices);
printf("flag\n");
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
    quickSort(currDA, 0, sizeDA(currDA)-1, 'u');
/*
    printf("%d->", n->value);
    displayDA(stdout, n->list);
    printf("\n");
*/
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

  printf("overall adjacencyList is: ");
  displayDA(stdout, adjacencyList);
  printf("\n");
  int x;
  for (x = 0; x < sizeDA(adjacencyList); x++) {
    NODE *curr = getDA(adjacencyList, x);
    printf("%d->", curr->value);
    displayDA(stdout, curr->list);
    printf("\n");
  }
  QUEUE *currQueue = newQUEUE(specialDisplayNODE);

  int size = sizeDA(adjacencyList);
  int level = 0;
  enqueue(currQueue, getDA(adjacencyList, 0));
  bool isRoot = true;
  int mainIndex = 1;
  int totalWeight = 0;
/*
printf("overall adjlist is: ");
displayDA(stdout, adjacencyList);
printf("\n");
int x;
for (x = 0; x < sizeDA(adjacencyList); x++) {
  DA *curr = getDA(adjacencyList, i)->list;
  displayDA(stdout, curr);
}
*/
  while (sizeQUEUE(currQueue) > 0) {
    QUEUE *nextQueue = newQUEUE(specialDisplayNODE);

    printf("%d:", level++);

//    printf("\nunsorted queue is: ");
//    displayQUEUE(stdout, currQueue);
//    printf("\n");
    sortQueue(currQueue);
//    printf("\ncurrQueue is: ");
//    displayQUEUE(stdout, currQueue);
//    printf("\n");

    if (isRoot) {
      NODE *currNode = dequeue(currQueue);
      //totalWeight += currNode->weight;
      displayNODE(currNode, 1);
/*
      printf("adjacencylist of currnode (%d) is: ", currNode->value);
      displayDA(stdout, currNode->list);
      printf("\n");
*/
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
/*
        printf("adjacencylist of currnode (%d) is: ", currNode->value);
        displayDA(stdout, currNode->list);
        printf("\n");
*/
        totalWeight += currNode->weight;
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

      currQueue = nextQueue;
      printf("\n");
    }

    if (sizeQUEUE(currQueue) == 0) {
      int i;
      for (i = mainIndex; i < size; i++) {
        NODE *currNode = getDA(adjacencyList, i);
        if (currNode->visited == 0) {
          enqueue(currQueue, currNode);
          level = 0;
          isRoot = true;
          printf("total weight: %d\n", totalWeight);
          totalWeight = 0;
          printf("----\n");
          break;
        }
      }
      mainIndex = i;
    }
  }

  printf("total weight: %d\n", totalWeight);
  totalWeight = 0;
  printf("----\n");
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

static void specialDisplayNODE(FILE *fp, void *v) {
  NODE *n = v;
  fprintf(fp, " %d(%d)%d", n->value, n->parent, n->weight);
}
static void addAdjacentNodes(DA *adjacencyList, DA *edgeArr) {
  //add currEdge adjacency info to adjacencyList
  int i;
  int size = sizeDA(edgeArr);
  int numVertices = sizeDA(adjacencyList);
  for (i = 0; i < size; i++) {
    EDGE *currEdge = getDA(edgeArr, i);
    //find node with value u in adjacencyList. add v to u's list
    int uIndex = binarySearchNodeIndex(adjacencyList, 0, numVertices, currEdge->u);
    NODE *uNode = getDA(adjacencyList, uIndex);
    NODE *vInsert = newNODE(currEdge->v, currEdge->u, currEdge->weight);
    insertDA(uNode->list, vInsert);
    //Setting the main version's attributes
    NODE *mainV = findMainVersion(adjacencyList, vInsert);
    mainV->parent = currEdge->u;
    mainV->weight = currEdge->weight;

    //find node with value v adjacencyList. Add u to v's list
    int vIndex = binarySearchNodeIndex(adjacencyList, 0, numVertices, currEdge->v);
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
//  e->u_index = index1;
//  e->v_index = index2;
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

  EDGE *tmp = edge1;
  edge1 = edge2;
  edge2 = tmp;
/*
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
printf("flag\n");
  EDGE *temp = newEDGE(u1, v1, weight1, udex1, vdex1);
printf("flag2\n");
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
*/
}

static void swapNodes(DA *arr, int index1, int index2) {
  NODE *node1 = getDA(arr, index1);
  NODE *node2 = getDA(arr, index2);
//  setDA(arr, index1, node2);
//  setDA(arr, index2, node1);
  NODE *tmp = newNODE(node1->value, node1->parent, node1->weight);
  tmp->list = node1->list;

  node1->value = node2->value;
  node1->parent = node2->parent;
  node1->weight = node2->weight;
  node1->list = node2->list;

  node2->value = tmp->value;
  node2->parent = tmp->parent;
  node2->weight = tmp->weight;
  node2->list = tmp->list;

}

static int partition(DA *arr, int low, int high, char e) {
/*
  int pivot;
  int i = 0;

  i = low - 1;


  if (e == 'e') {
    EDGE *curr = getDA(arr, high);
    pivot = curr->weight;
  }
  else if (e == 'u') {
    EDGE *curr = getDA(arr, high);
    pivot = curr->u;
  }
  else if (e == 'v') {
    EDGE *curr = getDA(arr, high);
    pivot = curr->v;
  }
  else if (e == 'n') {
    NODE *n = getDA(arr, high);
    pivot = n->value;
  }

  int j;
  for (j = low; j <= high; j++) {
    int currVal;
    if (e == 'e') {
      EDGE *curr = getDA(arr, j);
      currVal = curr->weight;
    }
    else if (e == 'u') {
      EDGE *curr = getDA(arr, j);
      currVal = curr->u;
    }
    else if (e == 'v') {
      EDGE *curr = getDA(arr, j);
      currVal = curr->v;
    }
    else if (e == 'n') {
      NODE *curr = getDA(arr, j);
      currVal = curr->value;
    }

    if (currVal <= pivot) {
      i += 1;
      if (e != 'n') {
        swap(arr, i, j);
      }
      else
        swapNodes(arr, i, j);
    }
  }
  if (e != 'n') {
    swap(arr, i+1, high);
  }
  else {
    swapNodes(arr, i+1, high);
  }
  printf("heller\n");
  i += 1;
  return i;
*/

  EDGE *pivot;
  NODE *pivotNode;
  if (e != 'n')
    pivot = getDA(arr, low);
  else
    pivotNode = getDA(arr, low);

  int leftWall = low;

  int i;
  for (i = low + 1; i < high; i++) {
    // if a[i] < pivot
    EDGE *edge_i;
    NODE *node_i;

    if (e != 'n')
      edge_i = getDA(arr, i);
    else
      node_i = getDA(arr, i);

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
    else if (e == 'n') {
      if (node_i->value < pivotNode->value) {
        leftWall += 1;
        swapNodes(arr, i, leftWall);
      }
    }
  }

  if (e != 'n')
    swap(arr, low, leftWall);
  else {
    swapNodes(arr, low, leftWall);
  }

  return ++leftWall;

}


static void quickSort(DA *arr, int low, int high, char e) {
  if (low < high) {
    if (e == 'e') {
      printf("low is: %d\n", low);
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
    else if (e == 'n') {
      int pivotLocation = partition(arr, low, high, 'n');
      quickSort(arr, low, pivotLocation-1, 'n');
      quickSort(arr, pivotLocation+1, high, 'n');
    }
  }
}

static void sortQueue(QUEUE *q) {
  DA *tmp = newDA(specialDisplayNODE);
  int size = sizeQUEUE(q);
  int i;
  for (i = 0; i < size; i++)
    insertDA(tmp, dequeue(q));
//printf("\nunsorted array is: ");
//displayDA(stdout, tmp);
//printf("\n");
  quickSort(tmp, 0, size-1, 'n');
//printf("sorted array is: ");
//displayDA(stdout, tmp);
//printf("\n");

  for (i = 0; i < size; i++)
    enqueue(q, getDA(tmp, i));
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
  quickSort(edgeArr, 0, numEdges-1, 'e');
printf("array sorted by edges is: ");
displayDA(stdout, edgeArr);
printf("\n");
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
