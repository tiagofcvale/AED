#include <stdlib.h>
#include <stdio.h>
#define SIZE 100

long int Fib_Cache[SIZE];

void initCache(void) {
    for (size_t i = 0; i < SIZE; i++) {
        Fib_Cache[i] = -1;
    }
}

int fibRec(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibRec(n-2) + fibRec(n-1);
}

int fibDin(int n) {
    long unsigned int fib[n+1];

    fib[0] = 0;
    fib[1] = 1;

    for (unsigned int i = 2; i <= n; i++) {
        fib[i] = fib[i - 2] + fib[i - 1];
    }

    return fib[n];
}

int fibMem(int n) {
    if(Fib_Cache[n] != -1) return Fib_Cache[n];
    long int r;
    if(n==0) r = 0;
    else if(n==1) r = 1;
    else r = fibMem(n-2) + fibMem(n-1);

    Fib_Cache[n] = r;
    return r;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s <numero>\n",argv[0]);
        return -1;
    }

    initCache();

    // printf("%d\n",fibRec(atoi(argv[1])));
    // printf("%d\n",fibDin(atoi(argv[1])));
    printf("%d\n",fibMem(atoi(argv[1])));
    return 0;
}