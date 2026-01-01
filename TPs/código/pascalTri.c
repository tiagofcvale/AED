#include <stdio.h>

int C(int n, int p) {
    if (p == 0 || p == n) return 1;
    return C(n-1, p-1) + C(n-1, p);
}

int main(int argc, char** argv) {
    printf("%d\n",C(4,2));
}