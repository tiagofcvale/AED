#include <stdio.h>
#include <assert.h>

int compare(int lista[], int n);

int main(void) {
    int lista[] = {1,2,3,4,5,6,7,8,9,10};
    int n = 10;

    int nElem = compare(lista, n);
    printf("N elementos de 1: %d\n", nElem);

    int lista1[] = {1, 2, 1, 4, 5, 6, 7, 8, 9, 10};

    int nElem1 = compare(lista1, n);
    printf("N elementos de 2: %d\n", nElem1);

    int lista2[] = {1, 2, 1, 3, 2, 6, 7, 8, 9, 10};

    int nElem2 = compare(lista2, n);
    printf("N elementos de 3: %d\n", nElem2);

    int lista3[] = {0, 2, 2, 0, 3, 3, 0, 4, 4, 0};

    int nElem3 = compare(lista3, n);
    printf("N elementos de 4: %d\n", nElem3);

    int lista4[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int nElem4 = compare(lista4, n);
    printf("N elementos de 5: %d\n", nElem4);

    return 0;
}

int compare(int lista[], int size) {
    assert(size > 2);
    
    int contador = 0, comp = 0;

    for (int i = 1; i < size - 1; i++) { 
        comp++;
        int soma = lista[i - 1] + lista[i + 1];
        if (soma == lista[i]) {
            contador++; 
        }
    }

    printf("Comparações: %d \n",comp);
    return contador;
}
