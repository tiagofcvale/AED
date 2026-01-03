//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira, Joao Manuel Rodrigues - May 2020, Nov 2024
//
// GraphDFS - RECURSIVE Depth-First Search
//

#ifndef _GRAPH_DFS_REC_
#define _GRAPH_DFS_REC_

#include "Graph.h"
#include "IntegersStack.h"

typedef struct _GraphDFSIter GraphDFSIter;

GraphDFSIter* GraphDFSIterExecute(Graph* g, unsigned int startVertex);

void GraphDFSIterDestroy(GraphDFSIter** p);

// Getting the result

unsigned int GraphDFSIterHasPathTo(const GraphDFSIter* p, unsigned int v);

Stack* GraphDFSIterPathTo(const GraphDFSIter* p, unsigned int v);

// DISPLAYING on the console

void GraphDFSIterShowPath(const GraphDFSIter* p, unsigned int v);

void GraphDFSIterDisplay(const GraphDFSIter* p);

#endif  // _GRAPH_DFS_REC_
