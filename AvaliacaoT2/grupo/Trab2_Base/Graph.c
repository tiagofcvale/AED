//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023, Nov/Dec 2024,
// Nov 2025
//
// Graph - Using a list of adjacency lists representation
//

// Student Name : Pedro Corado
// Student Number : 118781
// Student Name : Tiago Truta
// Student Number : 125996

/*** COMPLETE THE GraphGetSetAdjacentsTo FUNCTION */
/*** COMPLETE THE GraphComputeVertexWeights FUNCTION ***/
/*** COMPLETE THE GraphGetSubGraph FUNCTION ***/

#include "Graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "IndicesSet.h"
#include "SortedList.h"

struct _Vertex {
  unsigned int id;
  unsigned int inDegree;
  unsigned int outDegree;
  List* edgesList;
};

struct _Edge {
  unsigned int adjVertex;
  double weight;
};

struct _GraphHeader {
  int isDigraph;
  int isComplete;
  int isWeighted;
  unsigned int indicesRange;  // Indices in { 0, 1, ..., (indicesRange - 1) }
  unsigned int numVertices;
  unsigned int numEdges;
  IndicesSet* verticesSet;  // To store the vertex indices
  List* verticesList;
};

//
// The comparator for the VERTICES LIST
//
int graphVerticesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Vertex*)p1)->id;
  unsigned int v2 = ((struct _Vertex*)p2)->id;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

//
// The comparator for the EDGES LISTS
//
int graphEdgesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Edge*)p1)->adjVertex;
  unsigned int v2 = ((struct _Edge*)p2)->adjVertex;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

//
// Create an empty graph: 0 vertices, 0 edges
//
Graph* GraphCreateEmpty(unsigned int indicesRange, int isDigraph,
                        int isWeighted) {
  Graph* g = (Graph*)malloc(sizeof(struct _GraphHeader));
  if (g == NULL) abort();

  g->isDigraph = isDigraph;
  g->isComplete = 0;
  g->isWeighted = isWeighted;

  g->indicesRange = indicesRange;
  g->numVertices = 0;
  g->numEdges = 0;

  g->verticesSet = IndicesSetCreateEmpty(indicesRange);
  g->verticesList = ListCreate(graphVerticesComparator);

  assert((int)g->numVertices == (int)IndicesSetGetNumElems(g->verticesSet));
  assert((int)g->numVertices == ListGetSize(g->verticesList));

  return g;
}

//
// Create a graph with n vertices and 0 edges
//
Graph* GraphCreate(unsigned int numVertices, int isDigraph, int isWeighted) {
  Graph* g = (Graph*)malloc(sizeof(struct _GraphHeader));
  if (g == NULL) abort();

  g->isDigraph = isDigraph;
  g->isComplete = 0;
  g->isWeighted = isWeighted;

  g->indicesRange = numVertices;
  g->numVertices = numVertices;
  g->numEdges = 0;

  g->verticesSet = IndicesSetCreateFull(numVertices);
  g->verticesList = ListCreate(graphVerticesComparator);

  for (unsigned int i = 0; i < numVertices; i++) {
    struct _Vertex* v = (struct _Vertex*)malloc(sizeof(struct _Vertex));
    if (v == NULL) abort();

    v->id = i;
    v->inDegree = 0;
    v->outDegree = 0;

    v->edgesList = ListCreate(graphEdgesComparator);

    ListInsert(g->verticesList, v);
  }

  assert((int)g->numVertices == (int)IndicesSetGetNumElems(g->verticesSet));
  assert((int)g->numVertices == ListGetSize(g->verticesList));

  return g;
}

//
// Create a complete graph
//
Graph* GraphCreateComplete(unsigned int numVertices, int isDigraph) {
  Graph* g = GraphCreate(numVertices, isDigraph, 0);

  g->isComplete = 1;

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    for (unsigned int j = 0; j < g->numVertices; j++) {
      if (i == j) {
        continue;
      }
      struct _Edge* new = (struct _Edge*)malloc(sizeof(struct _Edge));
      if (new == NULL) abort();
      new->adjVertex = j;
      new->weight = 1;

      ListInsert(edges, new);
    }
    if (g->isDigraph) {
      v->inDegree = g->numVertices - 1;
      v->outDegree = g->numVertices - 1;
    } else {
      v->outDegree = g->numVertices - 1;
    }
  }
  if (g->isDigraph) {
    g->numEdges = numVertices * (numVertices - 1);
  } else {
    g->numEdges = numVertices * (numVertices - 1) / 2;
  }

  return g;
}

void GraphDestroy(Graph** p) {
  assert(*p != NULL);
  Graph* g = *p;

  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices) == 0) {
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
      struct _Vertex* v = ListGetCurrentItem(vertices);

      List* edges = v->edgesList;
      if (ListIsEmpty(edges) == 0) {
        int i = 0;
        ListMoveToHead(edges);
        for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
          struct _Edge* e = ListGetCurrentItem(edges);
          free(e);
        }
      }
      ListDestroy(&(v->edgesList));
      free(v);
    }
  }

  ListDestroy(&(g->verticesList));
  IndicesSetDestroy(&(g->verticesSet));
  free(g);

  *p = NULL;
}

// Read a graph from file
// Using the simple graph format of Sedgewick and Wayne
// Input argument must be a valid FILE POINTER
// File must be openend and closed by the caller
Graph* GraphFromFile(FILE* f) {
  assert(f != NULL);

  unsigned int isDigraph;
  fscanf(f, "%u", &isDigraph);

  unsigned int isWeighted;
  fscanf(f, "%u", &isWeighted);

  unsigned int numVertices;
  fscanf(f, "%u", &numVertices);

  unsigned int numEdges;
  fscanf(f, "%u", &numEdges);

  Graph* g = GraphCreate(numVertices, isDigraph, isWeighted);

  // Read the edges and add them to the graph
  unsigned int start_vertex;
  unsigned int end_vertex;
  double weight;

  if (isWeighted == 0) {
    for (unsigned int i = 0; i < numEdges; i++) {
      fscanf(f, "%u", &start_vertex);
      fscanf(f, "%u", &end_vertex);
      GraphAddEdge(g, start_vertex, end_vertex);
    }
  } else {
    for (unsigned int i = 0; i < numEdges; i++) {
      fscanf(f, "%u", &start_vertex);
      fscanf(f, "%u", &end_vertex);
      fscanf(f, "%lf", &weight);
      GraphAddWeightedEdge(g, start_vertex, end_vertex, weight);
    }
  }

  assert(numEdges == g->numEdges);

  return g;
}



Graph* GraphGetSubgraph(const Graph* g, IndicesSet* vertSet) {
  assert(IndicesSetIsSubset(vertSet, g->verticesSet));

  Graph* subgrafo = GraphCreateEmpty(g->indicesRange, g->isDigraph, g->isWeighted);

  // Adicionar vértices do conjunto ao subgrafo
  int primeiro_elem = IndicesSetGetFirstElem(vertSet);
  while (primeiro_elem != -1) {
    GraphAddVertex(subgrafo, primeiro_elem);
    primeiro_elem = IndicesSetGetNextElem(vertSet);
  }

  // Adicionar arestas que conectam vértices do conjunto
  primeiro_elem = IndicesSetGetFirstElem(vertSet);
  while (primeiro_elem != -1) {
    struct _Vertex dummy;
    dummy.id = primeiro_elem;
    ListSearch(g->verticesList, &dummy);
    struct _Vertex* vertice_orig = ListGetCurrentItem(g->verticesList);

    List* lista_adj = vertice_orig->edgesList;
    if (!ListIsEmpty(lista_adj)) {
      ListMoveToHead(lista_adj);
      for (int i = 0; i < ListGetSize(lista_adj); i++) {
        struct _Edge* aresta = ListGetCurrentItem(lista_adj);
        unsigned int adj = aresta->adjVertex;

        // Se o vértice adjacente também está no conjunto, adiciona a aresta
        if (IndicesSetContains(vertSet, adj)) {
          if (g->isWeighted) {
            GraphAddWeightedEdge(subgrafo, primeiro_elem, adj, aresta->weight);
          } else {
            GraphAddEdge(subgrafo, primeiro_elem, adj);
          }
        }
        ListMoveToNext(lista_adj);
      }
    }

    primeiro_elem = IndicesSetGetNextElem(vertSet);
  }

  GraphCheckInvariants(subgrafo);
  return subgrafo;
}

// Graph

int GraphIsDigraph(const Graph* g) { return g->isDigraph; }

int GraphIsComplete(const Graph* g) { return g->isComplete; }

int GraphIsWeighted(const Graph* g) { return g->isWeighted; }

unsigned int GraphGetVertexRange(const Graph* g) { return g->indicesRange; }

unsigned int GraphGetNumVertices(const Graph* g) { return g->numVertices; }

unsigned int GraphGetNumEdges(const Graph* g) { return g->numEdges; }

//
// For a graph
//
double GraphGetAverageDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return 2.0 * (double)g->numEdges / (double)g->numVertices;
}

static unsigned int _GetMaxDegree(const Graph* g) {
  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices)) return 0;

  unsigned int maxDegree = 0;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (v->outDegree > maxDegree) {
      maxDegree = v->outDegree;
    }
  }
  return maxDegree;
}

//
// For a graph
//
unsigned int GraphGetMaxDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return _GetMaxDegree(g);
}

//
// For a digraph
//
unsigned int GraphGetMaxOutDegree(const Graph* g) {
  assert(g->isDigraph == 1);
  return _GetMaxDegree(g);
}

// Vertices

int GraphAddVertex(Graph* g, unsigned int new_v) {
  assert(new_v < g->indicesRange);

  if (IndicesSetContains(g->verticesSet, new_v)) {
    return 0;
  }

  // One more vertex
  g->numVertices++;

  // Add to the vertices set
  IndicesSetAdd(g->verticesSet, new_v);

  // The vertex data for the vertex list
  struct _Vertex* v = (struct _Vertex*)malloc(sizeof(struct _Vertex));
  if (v == NULL) abort();

  v->id = new_v;
  v->inDegree = 0;
  v->outDegree = 0;
  v->edgesList = ListCreate(graphEdgesComparator);

  // Add to the vertices list
  ListInsert(g->verticesList, v);

  return 1;
}

//
// returns the set of vertices
// as a set of vertex indices
// Deep copy of the set of vertices indices
//
IndicesSet* GraphGetSetVertices(const Graph* g) {
  IndicesSet* set = IndicesSetCreateCopy(g->verticesSet);
  return set;
}



IndicesSet* GraphGetSetAdjacentsTo(const Graph* g, unsigned int v) {
  assert(IndicesSetContains(g->verticesSet, (uint16_t)v));

  IndicesSet* adjacentes_set = IndicesSetCreateEmpty(g->indicesRange);

  // Encontrar o vértice v na lista de vértices
  struct _Vertex dummy;
  dummy.id = v;
  ListSearch(g->verticesList, &dummy);
  struct _Vertex* vertice_atual = ListGetCurrentItem(g->verticesList);

  // Percorrer a lista de adjacências do vértice v
  List* lista_adj = vertice_atual->edgesList;
  if (!ListIsEmpty(lista_adj)) {
    ListMoveToHead(lista_adj);
    for (int i = 0; i < ListGetSize(lista_adj); i++) {
      struct _Edge* aresta = ListGetCurrentItem(lista_adj);
      IndicesSetAdd(adjacentes_set, aresta->adjVertex);
      ListMoveToNext(lista_adj);
    }
  }

  return adjacentes_set;
}



double* GraphComputeVertexWeights(const Graph* g) {
  double* pesos = malloc(g->indicesRange * sizeof(double));
  if (pesos == NULL) abort();

  // Inicializar com -1 para vértices inexistentes
  for (unsigned int v = 0; v < g->indicesRange; v++) {
    pesos[v] = -1.0;
  }

  // Percorrer todos os vértices do grafo
  ListMoveToHead(g->verticesList);
  for (unsigned int i = 0; i < g->numVertices; i++) {
    struct _Vertex* vertice = ListGetCurrentItem(g->verticesList);
    unsigned int id = vertice->id;
    double peso_total = 0.0;

    // Percorrer arestas adjacentes
    List* lista_adj = vertice->edgesList;
    if (!ListIsEmpty(lista_adj)) {
      ListMoveToHead(lista_adj);
      for (int j = 0; j < ListGetSize(lista_adj); j++) {
        struct _Edge* aresta = ListGetCurrentItem(lista_adj);
        peso_total += aresta->weight;
        ListMoveToNext(lista_adj);
      }
    } else {
      // Se não houver arestas, peso é zero
      peso_total = 0.0;
    }

    pesos[id] = peso_total;
    ListMoveToNext(g->verticesList);
  }

  return pesos;
}

//
// For a graph
//
unsigned int GraphGetVertexDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 0);
  assert(IndicesSetContains(g->verticesSet, v));

  struct _Vertex search_dummy;
  search_dummy.id = v;
  ListSearch(g->verticesList, (void*)(&search_dummy));
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexOutDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(IndicesSetContains(g->verticesSet, v));

  struct _Vertex search_dummy;
  search_dummy.id = v;
  ListSearch(g->verticesList, (void*)(&search_dummy));
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexInDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(IndicesSetContains(g->verticesSet, v));

  struct _Vertex search_dummy;
  search_dummy.id = v;
  ListSearch(g->verticesList, (void*)(&search_dummy));
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->inDegree;
}

// Edges

static int _addEdge(Graph* g, unsigned int v, unsigned int w, double weight) {
  // Insert edge (v,w)
  struct _Edge* edge_v_w = (struct _Edge*)malloc(sizeof(struct _Edge));
  if (edge_v_w == NULL) abort();
  edge_v_w->adjVertex = w;
  edge_v_w->weight = weight;

  struct _Vertex search_dummy;
  search_dummy.id = v;
  ListSearch(g->verticesList, (void*)(&search_dummy));
  struct _Vertex* vertex_v = ListGetCurrentItem(g->verticesList);
  int result = ListInsert(vertex_v->edgesList, edge_v_w);

  if (result == -1) {
    // Insertion failed --- Destroy the allocated edge
    free(edge_v_w);
    return 0;
  }

  // Update
  g->numEdges++;
  vertex_v->outDegree++;

  search_dummy.id = w;
  ListSearch(g->verticesList, (void*)(&search_dummy));
  struct _Vertex* vertex_w = ListGetCurrentItem(g->verticesList);
  // DIRECTED GRAPH --- Update the in-degree of vertex w
  if (g->isDigraph == 1) {
    vertex_w->inDegree++;
  }

  // If UNDIRECTED GRAPH
  if (g->isDigraph == 0) {
    // It is a BIDIRECTIONAL EDGE --- Insert edge (w,v)
    struct _Edge* edge_w_v = (struct _Edge*)malloc(sizeof(struct _Edge));
    if (edge_w_v == NULL) abort();
    edge_w_v->adjVertex = v;
    edge_w_v->weight = weight;

    result = ListInsert(vertex_w->edgesList, edge_w_v);

    if (result == -1) {
      // Insertion failed --- Destroy the allocated edge
      free(edge_w_v);

      // And remove the edge (v,w) that was inserted above
      ListSearch(vertex_v->edgesList, (void*)edge_v_w);
      ListRemoveCurrent(vertex_v->edgesList);

      // UNDO the updates
      g->numEdges--;
      vertex_v->outDegree--;
      vertex_w->inDegree--;

      return 0;
    } else {
      // g->numEdges++; // Do not count the same edge twice on a undirected
      // graph !!

      // Just update the outDegree of vertex w
      vertex_w->outDegree++;
    }
  }

  return 1;
}

int GraphAddEdge(Graph* g, unsigned int v, unsigned int w) {
  assert(g->isWeighted == 0);
  assert(v != w);
  assert(IndicesSetContains(g->verticesSet, v));
  assert(IndicesSetContains(g->verticesSet, w));

  return _addEdge(g, v, w, 1.0);
}

int GraphAddWeightedEdge(Graph* g, unsigned int v, unsigned int w,
                         double weight) {
  assert(g->isWeighted == 1);
  assert(v != w);
  assert(IndicesSetContains(g->verticesSet, v));
  assert(IndicesSetContains(g->verticesSet, w));

  return _addEdge(g, v, w, weight);
}

// CHECKING

int GraphCheckInvariants(const Graph* g) {
  assert(g != NULL);

  assert(g->isComplete == 0 || g->isComplete == 1);
  assert(g->isDigraph == 0 || g->isDigraph == 1);
  assert(g->isWeighted == 0 || g->isWeighted == 1);

  if (g->isComplete) {
    assert(g->indicesRange == g->numVertices);
    unsigned int n = g->numVertices;
    if (g->isDigraph) {
      assert(g->numEdges == n * (n - 1));
    } else {
      assert(g->numEdges == n * (n - 1) / 2);
    }
  }

  assert((int)g->numVertices == ListGetSize(g->verticesList));
  assert((int)g->numVertices == IndicesSetGetNumElems(g->verticesSet));

  // Checking the vertices list
  ListTestInvariants(g->verticesList);

  // Checking the total number of edges
  unsigned int out_degree_total = 0;
  unsigned int in_degree_total = 0;

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    assert(IndicesSetContains(g->verticesSet, v->id));
    out_degree_total += v->outDegree;
    if (g->isDigraph) {
      in_degree_total += v->inDegree;
    }
  }

  if (g->isDigraph) {
    assert(in_degree_total == out_degree_total);
    assert(g->numEdges == out_degree_total);
  } else {
    // Unidrected graph
    assert(g->numEdges == out_degree_total / 2);
  }

  // For each vertex, checking its adjacency list
  ListMoveToHead(vertices);
  i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    ListTestInvariants(edges);
    assert((int)v->outDegree == ListGetSize(edges));
  }

  return 0;
}

// DISPLAYING on the console

void GraphDisplay(const Graph* g) {
  printf("---\n");
  if (g->isWeighted) {
    printf("Weighted ");
  }
  if (g->isComplete) {
    printf("COMPLETE ");
  }
  if (g->isDigraph) {
    printf("Digraph\n");
    printf("Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    printf("Graph\n");
    printf("Max Degree = %d\n", GraphGetMaxDegree(g));
  }
  printf("Vertices = %2d | Edges = %2d\n", g->numVertices, g->numEdges);

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    printf("%2d ->", v->id);

    if (ListIsEmpty(v->edgesList)) {
      printf("\n");
    } else {
      List* edges = v->edgesList;
      int i = 0;
      ListMoveToHead(edges);
      for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
        struct _Edge* e = ListGetCurrentItem(edges);
        if (g->isWeighted) {
          printf("   %2d(%4.2f)", e->adjVertex, e->weight);
        } else {
          printf("   %2d", e->adjVertex);
        }
      }
      printf("\n");
      // Checking the invariants of the list of edges
      ListTestInvariants(edges);
    }
  }
  printf("---\n");
  // Checking the invariants of the list of vertices
  ListTestInvariants(vertices);
}

void GraphListAdjacents(const Graph* g, unsigned int v) {
  printf("---\n");

  IndicesSet* adjacents_set = GraphGetSetAdjacentsTo(g, v);

  printf("Vertex %d adjacent vertices :\n", v);

  IndicesSetDisplay(adjacents_set);

  IndicesSetDestroy(&adjacents_set);

  printf("---\n");
}

// Display the graph in DOT language.
// To draw the graph, you can use dot (from Graphviz) or paste result on:
//   https://dreampuf.github.io/GraphvizOnline
void GraphDisplayDOT(const Graph* g) {
  printf("// Paste in: https://dreampuf.github.io/GraphvizOnline\n");
  GraphToDOTFile(g, stdout);
}

// Save the graph in DOT format
void GraphToDOTFile(const Graph* g, FILE* f) {
  assert(f != NULL);
  char* gtypes[] = {"graph", "digraph"};
  char* edgeops[] = {"--", "->"};
  char* gtype = gtypes[g->isDigraph];
  char* edgeop = edgeops[g->isDigraph];

  fprintf(f, "%s {\n", gtype);
  fprintf(f, "  // Vertices = %2d\n", g->numVertices);
  fprintf(f, "  // Edges = %2d\n", g->numEdges);
  if (g->isDigraph) {
    fprintf(f, "  // Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    fprintf(f, "  // Max Degree = %d\n", GraphGetMaxDegree(g));
  }

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    fprintf(f, "  %d", v->id);

    List* edges = v->edgesList;
    int k = 0;
    ListMoveToHead(edges);
    for (; k < ListGetSize(edges); ListMoveToNext(edges), k++) {
      struct _Edge* e = ListGetCurrentItem(edges);
      unsigned int j = e->adjVertex;
      if (g->isDigraph || v->id <= j) {  // for graphs, draw only 1 edge
        fprintf(f, "  %d %s %d", v->id, edgeop, j);
        if (g->isWeighted) {
          fprintf(f, " [label=%4.2f]", e->weight);
        }
      }
    }
    fprintf(f, "\n");
  }
  fprintf(f, "}\n");
}
