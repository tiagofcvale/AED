#include <stdio.h>
#include <stdlib.h>

int seqSearch(int a[], int size, int n, int i);

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Uso: %s <numero>\n", argv[0]);
        return 1;
    }
    
    int a[] = {1,2,3,4,5,6,7,9,0};
    printf("%d\n",seqSearch(a,9,atoi(argv[1]),0));
    return 0;
}

int seqSearch(int a[], int size, int n, int i) {
    if (i == size) return -1;
    if (a[i] == n) return i;
    return seqSearch(a, size, n, i + 1);
}