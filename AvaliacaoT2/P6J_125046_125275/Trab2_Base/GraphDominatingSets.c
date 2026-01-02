//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : Alan Marques
// Student Number : 125046
// Student Name : Artur Yavorskyy
// Student Number : 125275

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"

// Check if the given set is a dominating set for the graph
// Return 1 if true, or 0 otherwise
//
// A dominating set is a set of graph vertices such that every
// vertex not in the set is adjacent to at least one vertex in it
int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);
  assert(vertSet != NULL);
  assert(IndicesSetIsEmpty(vertSet) == 0);

  IndicesSet* allVertices = GraphGetSetVertices(g);
  int v = IndicesSetGetFirstElem(allVertices);

  while (v != -1) {
    // If v is not in the dominating set, it must have a neighbor in it
    if (!IndicesSetContains(vertSet, v)) {
      IndicesSet* neighbors = GraphGetSetAdjacentsTo(g, v);

      // Keep only neighbors that belong to vertSet
      IndicesSetIntersection(neighbors, vertSet);

      if (IndicesSetIsEmpty(neighbors)) {
        IndicesSetDestroy(&neighbors);
        IndicesSetDestroy(&allVertices);
        return 0;
      }

      IndicesSetDestroy(&neighbors);
    }
    v = IndicesSetGetNextElem(allVertices);
  }

  IndicesSetDestroy(&allVertices);
  return 1;
}

// Compute a MINIMUM VERTEX DOMINATING SET
// using an EXHAUSTIVE SEARCH approach
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  IndicesSet* bestSet = GraphGetSetVertices(g);
  int bestSize = IndicesSetGetNumElems(bestSet);

  IndicesSet* candidate =
      IndicesSetCreateEmpty(GraphGetVertexRange(g));

  do {
    if (IndicesSetIsEmpty(candidate)) {
      continue;
    }

    int candidateSize = IndicesSetGetNumElems(candidate);

    if (candidateSize >= bestSize) {
      continue;
    }

    if (GraphIsDominatingSet(g, candidate)) {
      bestSize = candidateSize;
      IndicesSetDestroy(&bestSet);
      bestSet = IndicesSetCreateCopy(candidate);
    }

  } while (IndicesSetNextSubset(candidate));

  IndicesSetDestroy(&candidate);
  return bestSet;  // ownership passed to caller
}

// Compute a MINIMUM WEIGHT VERTEX DOMINATING SET
// using an EXHAUSTIVE SEARCH approach
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  double* vertexWeights = GraphComputeVertexWeights(g);
  assert(vertexWeights != NULL);

  // Initial solution: all vertices
  IndicesSet* bestSet = GraphGetSetVertices(g);
  double bestWeight = 0.0;

  int v = IndicesSetGetFirstElem(bestSet);
  while (v != -1) {
    if (vertexWeights[v] >= 0.0) {
      bestWeight += vertexWeights[v];
    }
    v = IndicesSetGetNextElem(bestSet);
  }

  IndicesSet* candidate =
      IndicesSetCreateEmpty(GraphGetVertexRange(g));

  do {
    if (IndicesSetIsEmpty(candidate)) {
      continue;
    }

    double currentWeight = 0.0;

    // Compute weight with early pruning
    int cv = IndicesSetGetFirstElem(candidate);
    while (cv != -1) {
      if (vertexWeights[cv] >= 0.0) {
        currentWeight += vertexWeights[cv];
        if (currentWeight >= bestWeight) {
          break;  // prune
        }
      }
      cv = IndicesSetGetNextElem(candidate);
    }

    if (currentWeight >= bestWeight) {
      continue;
    }

    if (GraphIsDominatingSet(g, candidate)) {
      bestWeight = currentWeight;
      IndicesSetDestroy(&bestSet);
      bestSet = IndicesSetCreateCopy(candidate);
    }

  } while (IndicesSetNextSubset(candidate));

  free(vertexWeights);
  IndicesSetDestroy(&candidate);
  return bestSet;  // ownership passed to caller
}