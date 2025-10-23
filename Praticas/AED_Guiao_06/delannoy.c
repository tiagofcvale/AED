#include <stdio.h>
#include <stdlib.h>
#define SIZE 100 

long delannoy(int, int);
unsigned long ADDS;
unsigned long delannoy_iteractive(int, int);
long int delCache[SIZE];

void initCache(void) {
    for (size_t i = 0; i < SIZE; i++) {
        delCache[i] = -1;
    }
}

int main(int argc, char *argv[]) { 

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    unsigned long res = delannoy(m,n);
    printf("Resultado = %ld\n",res);
    printf("ADDS: %ld\n\n",ADDS);
    ADDS = 0;

    unsigned long resultado = delannoy_iteractive(m, n);
    printf("D(%d, %d) = %lu\n", m, n, resultado);
    printf("ADDS: %ld\n",ADDS);
    ADDS=0;

    return 0;
}

long delannoy(int m, int n) {
    if (m == 0 || n == 0) {
        return 1;
    }
    ADDS+=2;
    return (delannoy(m-1,n) + delannoy(m-1,n-1) + delannoy(m,n-1));
}


unsigned long delannoy_iteractive(int m, int n) {
    // Array 2D
    unsigned long D[m + 1][n + 1];

    // Casos iniciais: D[i][0] = D[0][j] = 1
    for (int i = 0; i <= m; i++)
        D[i][0] = 1;

    for (int j = 0; j <= n; j++)
        D[0][j] = 1;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            ADDS+=2;
            D[i][j] = D[i - 1][j] + D[i - 1][j - 1] + D[i][j - 1];
        }
    }

    return D[m][n];
}

// O MAIOR NÚMERO QUE CONSIGO DETERMINAR É 14