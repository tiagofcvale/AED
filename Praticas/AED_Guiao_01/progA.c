#include <stdio.h>

int main(void) {
    
    void cumsum(int a[], int b[]);
    void printArray(char* c, int a[]);

    int a[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    printArray("a", a);

    int b[12];
    cumsum(a, b);

    printArray("b", b);

    return 0;
}

void cumsum(int a[], int b[]) {
    b[0] = a[0];
    for (int i = 1; i < 12; i++) {
        b[i] = b[i - 1] + a[i];
    }
}

void printArray(char* c, int a[]) {
    printf("%s: ", c);
    for (int i = 0; i < 12; i++) {
        printf("%d,", a[i]);
    }
    printf("\n");
}
