#include <stdio.h>

int delannoy(int, int);

int main() {
    int m = 2, n = 3;  
    int res = delannoy(m,n);
    printf("Resultado = %d\n",res);
}

int delannoy(int m, int n) {
    if (m == 0 || n == 0) {
        return 1;
    }
    return (delannoy(m-1,n) + delannoy(m-1,n-1) + delannoy(m,n-1));
}