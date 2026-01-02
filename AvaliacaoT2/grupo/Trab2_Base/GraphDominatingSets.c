//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : Pedro Corado
// Student Number : 118781
// Student Name : Tiago Truta
// Student Number : 125996

/*** COMPLETE THE GraphIsDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinWeightDominatingSet FUNCTION ***/

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"



int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);
  assert(IndicesSetIsEmpty(vertSet) == 0);

  IndicesSet* todos_vertices = GraphGetSetVertices(g);
  IndicesSet* dominados = IndicesSetCreateCopy(vertSet);

  // Para cada vértice no conjunto, adicionar os seus vizinhos ao conjunto dominado
  int elem = IndicesSetGetFirstElem(vertSet);
  while (elem != -1) {
    IndicesSet* vizinhos = GraphGetSetAdjacentsTo(g, elem);
    IndicesSetUnion(dominados, vizinhos);
    IndicesSetDestroy(&vizinhos);
    elem = IndicesSetGetNextElem(vertSet);
  }

  // Verificar se todos os vértices do grafo estão dominados
  int resultado = IndicesSetIsSubset(todos_vertices, dominados);

  IndicesSetDestroy(&todos_vertices);
  IndicesSetDestroy(&dominados);

  return resultado;
}


IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  double* pesos = GraphComputeVertexWeights(g);
  unsigned int range = GraphGetVertexRange(g);
  IndicesSet* melhor_conjunto = NULL;
  double menor_peso_total = 1e30;  // Em vez de INFINITY

  IndicesSet* conjunto_atual = IndicesSetCreateEmpty(range);

  // Gerar todos os subconjuntos
  do {
    if (IndicesSetIsEmpty(conjunto_atual)) continue;

    if (GraphIsDominatingSet(g, conjunto_atual)) {
      double peso_total = 0.0;
      int elem = IndicesSetGetFirstElem(conjunto_atual);
      while (elem != -1) {
        if (pesos[elem] >= 0) {  // Só somar se o vértice existe
          peso_total += pesos[elem];
        }
        elem = IndicesSetGetNextElem(conjunto_atual);
      }

      if (peso_total < menor_peso_total) {
        menor_peso_total = peso_total;
        if (melhor_conjunto != NULL) {
          IndicesSetDestroy(&melhor_conjunto);
        }
        melhor_conjunto = IndicesSetCreateCopy(conjunto_atual);
      }
    }
  } while (IndicesSetNextSubset(conjunto_atual));

  IndicesSetDestroy(&conjunto_atual);
  free(pesos);

  if (melhor_conjunto == NULL) {
    melhor_conjunto = IndicesSetCreateEmpty(range);
  }

  return melhor_conjunto;
}




