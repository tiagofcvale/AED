    #include <stdio.h>
    #include <stdlib.h>
    #define SIZE 100 

    long delannoy(int, int);
    unsigned long ADDS;
    unsigned long delannoy_dinamic(int, int);
    long int delCache[SIZE][SIZE];
    unsigned long delannoy_memoization(int, int);

    int main(int argc, char *argv[]) { 

        int m = atoi(argv[1]);
        int n = atoi(argv[2]);

        unsigned long res = delannoy(m,n);
        printf("Resultado = %ld\n",res);
        printf("ADDS: %ld\n\n",ADDS);
        ADDS = 0;

        unsigned long resd = delannoy_dinamic(m, n);
        printf("D(%d, %d) = %lu\n", m, n, resd);
        printf("ADDS: %ld\n\n",ADDS);
        ADDS=0;

        // INICIALIZAR delCache 
        for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                delCache[i][j] = -1;
            }
        }

        unsigned long resm = delannoy_memoization(m, n);
        printf("D(%d, %d) = %lu\n", m, n, resm);
        printf("ADDS: %ld\n",ADDS);
        return 0;
    }

    long delannoy(int m, int n) {
        if (m == 0 || n == 0) {
            return 1;
        }
        ADDS+=2;
        return (delannoy(m-1,n) + delannoy(m-1,n-1) + delannoy(m,n-1));
    }


    unsigned long delannoy_dinamic(int m, int n) {
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

    unsigned long delannoy_memoization(int m, int n) {
        if (delCache[m][n] != -1) return delCache[m][n];

        if (m == 0 || n == 0) {
            return delCache[m][n] = 1;
        } else {
            ADDS+=2;
            delCache[m][n] =
                delannoy_memoization(m - 1, n)
                + delannoy_memoization(m - 1, n - 1)
                + delannoy_memoization(m, n - 1);
        }
        
        return (delCache[m][n]);
    }

    // O MAIOR NÚMERO QUE CONSIGO DETERMINAR É 14
