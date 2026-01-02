#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Graph.h"
#include "GraphDominatingSets.h"

/* Adiciona m arestas aleatórias sem lacetes nem repetições */
static void add_random_edges(Graph* g, int n, int m, int weighted) {
    int added = 0;

    while (added < m) {
        int u = rand() % n;
        int v = rand() % n;
        if (u == v) continue;

        if (weighted) {
            double w = (double)(rand() % 10 + 1); // pesos 1..10
            if (GraphAddWeightedEdge(g, u, v, w))
                added++;
        } else {
            if (GraphAddEdge(g, u, v))
                added++;
        }
    }
}

int main() {
    const int n_min = 4;
    const int n_max = 26;

    srand((unsigned int)time(NULL));

    printf("=== Testes GraphComputeMinDominatingSet (|E| ≈ n/2) ===\n");

    int case_num = 1;
    for (int n = n_min; n <= n_max; n++) {
        int m = n / 2;

        Graph* g = GraphCreate(n, 0, 0);
        add_random_edges(g, n, m, 0);

        clock_t start = clock();
        IndicesSet* s = GraphComputeMinDominatingSet(g);
        clock_t end = clock();

        double time_sec = (double)(end - start) / CLOCKS_PER_SEC;

        printf("Caso %2d: n=%2d, |V|=%2u, |E|=%2u, tempo=%.6f s\n",
               case_num++, n,
               GraphGetNumVertices(g),
               GraphGetNumEdges(g),
               time_sec);

        IndicesSetDestroy(&s);
        GraphDestroy(&g);
    }

    printf("\n=== Testes GraphComputeMinWeightDominatingSet (|E| ≈ n/2) ===\n");

    case_num = 1;
    for (int n = n_min; n <= n_max; n++) {
        int m = n / 2;

        Graph* g = GraphCreate(n, 0, 1);
        add_random_edges(g, n, m, 1);

        clock_t start = clock();
        IndicesSet* s = GraphComputeMinWeightDominatingSet(g);
        clock_t end = clock();

        double time_sec = (double)(end - start) / CLOCKS_PER_SEC;

        printf("Caso %2d: n=%2d, |V|=%2u, |E|=%2u, tempo=%.6f s\n",
               case_num++, n,
               GraphGetNumVertices(g),
               GraphGetNumEdges(g),
               time_sec);

        IndicesSetDestroy(&s);
        GraphDestroy(&g);
    }

    return 0;
}