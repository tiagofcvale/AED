//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : Rodrigo Gonçalves
// Student Number : 124750
// Student Name : Rodrigo Silva
// Student Number : 125171

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h> // Para DBL_MAX

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"

//
// Check if the given set is a dominating set for the graph
// Return 1 if true, or 0 otherwise
//
int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);
  assert(IndicesSetIsEmpty(vertSet) == 0);

  // Percorre todos os vértices do grafo
  IndicesSet* allVertices = GraphGetSetVertices(g);
  int v = IndicesSetGetFirstElem(allVertices);

  while (v != -1) {
    // Se v já pertence ao conjunto, está dominado
    if (IndicesSetContains(vertSet, v)) {
      v = IndicesSetGetNextElem(allVertices);
      continue;
    }

    // Caso contrário, verifica se algum vizinho está no conjunto
    IndicesSet* neighbors = GraphGetSetAdjacentsTo(g, v);
    int neighbor = IndicesSetGetFirstElem(neighbors);
    int isDominatedByNeighbor = 0;

    while (neighbor != -1) {
      if (IndicesSetContains(vertSet, neighbor)) {
        isDominatedByNeighbor = 1; // vizinho domina v
        break;
      }
      neighbor = IndicesSetGetNextElem(neighbors);
    }

    IndicesSetDestroy(&neighbors);

    // Se não estiver dominado, não é conjunto dominante
    if (!isDominatedByNeighbor) {
      IndicesSetDestroy(&allVertices);
      return 0;
    }

    v = IndicesSetGetNextElem(allVertices);
  }

  IndicesSetDestroy(&allVertices);
  return 1; // Todos os vértices foram verificados com sucesso
}

//
// Compute a MIN VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the/a dominating set
//
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  unsigned int range = GraphGetVertexRange(g);
  IndicesSet* candidate = IndicesSetCreateEmpty(range);
  
  // Vamos guardar aqui o melhor conjunto encontrado
  // Inicializamos com NULL ou podíamos começar com o conjunto cheio
  IndicesSet* bestSet = NULL;
  unsigned int minSize = range + 1; // Valor impossivelmente alto para começar

  // Iterar por TODOS os subconjuntos possíveis (2^N)
  // O loop do-while com NextSubset garante que testamos todas as combinações
  do {
    // Ignora o conjunto vazio
    if (IndicesSetIsEmpty(candidate)) {
        continue;
    }

    // Verifica se é dominante
    if (GraphIsDominatingSet(g, candidate)) {
      unsigned int currentSize = IndicesSetGetNumElems(candidate);

      // Se for menor, atualiza melhor
      if (currentSize < minSize) {
        minSize = currentSize;

        if (bestSet != NULL) {
          IndicesSetDestroy(&bestSet);
        }
        bestSet = IndicesSetCreateCopy(candidate);

        // Otimização: não pode ficar menor que 1
        if (minSize == 1) break;
      }
    }

  } while (IndicesSetNextSubset(candidate));

  IndicesSetDestroy(&candidate);

  // Se nada encontrado, devolve conjunto vazio
  if (bestSet == NULL) {
      return IndicesSetCreateEmpty(range);
  }

  return bestSet;
}

//
// Compute a MIN WEIGHT VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the dominating set
//
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  // Calcula os pesos dos vértices
  double* vertexWeights = GraphComputeVertexWeights(g);

  unsigned int range = GraphGetVertexRange(g);
  IndicesSet* candidate = IndicesSetCreateEmpty(range);
  IndicesSet* bestSet = NULL;

  double minTotalWeight = DBL_MAX;

  // Percorre todos os subconjuntos
  do {
    if (IndicesSetIsEmpty(candidate)) {
        continue;
    }

    // Verifica se é dominante
    if (GraphIsDominatingSet(g, candidate)) {

      // Soma os pesos do candidato
      double currentWeight = 0.0;
      int v = IndicesSetGetFirstElem(candidate);
      while(v != -1) {
          if (vertexWeights[v] != -1.0) {
            currentWeight += vertexWeights[v];
          }
          v = IndicesSetGetNextElem(candidate);
      }

      // Atualiza melhor se o peso for menor
      if (currentWeight < minTotalWeight) {
        minTotalWeight = currentWeight;

        if (bestSet != NULL) {
          IndicesSetDestroy(&bestSet);
        }
        bestSet = IndicesSetCreateCopy(candidate);
      }
    }

  } while (IndicesSetNextSubset(candidate));

  free(vertexWeights);
  IndicesSetDestroy(&candidate);

  // Se nada encontrado, devolve conjunto vazio
  if (bestSet == NULL) {
      return IndicesSetCreateEmpty(range);
  }

  return bestSet;
}