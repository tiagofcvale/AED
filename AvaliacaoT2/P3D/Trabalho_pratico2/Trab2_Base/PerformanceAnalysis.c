//
// PerformanceAnalysis.c
// 
// Ficheiro auxiliar para gerar dados para o Relatório de Complexidade
// AED 2025/2026
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "Graph.h"
#include "GraphDominatingSets.h"
#include "IndicesSet.h"

// Função auxiliar para gerar um grafo aleatório
// Recebe o número de vértices e a probabilidade de criar uma aresta (0.0 a 1.0)
Graph* CreateRandomGraph(unsigned int numVertices, double probability) {
    Graph* g = GraphCreate(numVertices, 0, 0); // 0 = Não orientado, 0 = Sem pesos
    
    for (unsigned int i = 0; i < numVertices; i++) {
        for (unsigned int j = i + 1; j < numVertices; j++) {
            // Gera um número aleatório entre 0 e 1
            double r = (double)rand() / (double)RAND_MAX;
            if (r < probability) {
                GraphAddEdge(g, i, j);
            }
        }
    }
    return g;
}

int main(void) {
    // Semente para os números aleatórios
    srand(time(NULL));

    printf("=== ANALISE DE COMPLEXIDADE: CONJUNTOS DOMINANTES ===\n");
    printf("Algoritmo: Procura Exaustiva (Brute Force)\n\n");
    
    // Cabeçalho da Tabela
    printf("%-10s | %-15s | %-15s | %-15s\n", "Vertices", "Tempo (s)", "Iteracoes (Est)", "Resultado Size");
    printf("-------------------------------------------------------------\n");

    // Vamos testar de 5 até 25 vértices (CUIDADO: acima de 22-23 pode demorar muito!)
    // Podes ajustar o limite 'v <= 25' se o teu PC for rápido/lento
    for (unsigned int v = 5; v <= 22; v++) {
        
        // 1. Criar Grafo Aleatório (Probabilidade de aresta 50%)
        Graph* g = CreateRandomGraph(v, 0.5);
        
        // 2. Medir o Tempo
        clock_t inicio = clock();
        
        // Chamada ao algoritmo que queremos testar
        IndicesSet* result = GraphComputeMinDominatingSet(g);
        
        clock_t fim = clock();
        
        // 3. Calcular métricas
        double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        unsigned int tamanho_resultado = IndicesSetGetNumElems(result);
        
        // Estimativa de iterações (2^N) apenas para referência no relatório
        // Usamos long long para números grandes
        unsigned long long estimativa_ops = 1ULL << v; 

        // 4. Imprimir Linha da Tabela
        printf("%-10u | %-15.6f | %-15llu | %-15u\n", 
               v, tempo_gasto, estimativa_ops, tamanho_resultado);
        
        // Limpeza
        IndicesSetDestroy(&result);
        GraphDestroy(&g);
    }

    printf("-------------------------------------------------------------\n");
    printf("Nota: A coluna 'Iteracoes' eh o valor teorico 2^N.\n");
    printf("Se o tempo for 0.000000, aumenta o tamanho do grafo.\n");

    return 0;
}