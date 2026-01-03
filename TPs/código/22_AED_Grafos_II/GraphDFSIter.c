#include "GraphDFSIter.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"

// Estrutura para a versão iterativa
struct _GraphDFSIter {
  unsigned int* marked;
  int* predecessor;
  Graph* graph;
  unsigned int startVertex;
};

GraphDFSIter* GraphDFSIterExecute(Graph* g, unsigned int startVertex) {
  assert(g != NULL);
  unsigned int numVertices = GraphGetNumVertices(g);

  GraphDFSIter* result = (GraphDFSIter*)malloc(sizeof(struct _GraphDFSIter));
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  for (unsigned int i = 0; i < numVertices; i++) result->predecessor[i] = -1;

  result->graph = g;
  result->startVertex = startVertex;

  // Criar a Pilha para a travessia
  Stack* stack = StackCreate(numVertices);
  StackPush(stack, startVertex);

  while (!StackIsEmpty(stack)) {
    unsigned int v = StackPop(stack);

    if (result->marked[v] == 0) {
      result->marked[v] = 1;

      unsigned int* neighbors = GraphGetAdjacentsTo(g, v);
      // Para manter a mesma ordem da recursão, 
      // os vizinhos devem ser empilhados de trás para frente
      for (int i = (int)neighbors[0]; i >= 1; i--) {
        unsigned int w = neighbors[i];
        if (result->marked[w] == 0) {
          result->predecessor[w] = v;
          StackPush(stack, w);
        }
      }
      free(neighbors);
    }
  }
  StackDestroy(&stack);
  return result;
}