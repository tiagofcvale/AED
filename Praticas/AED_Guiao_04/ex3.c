#include <stdio.h>
#include <assert.h>

int iSum(int[], int);

int main(void) {
    int lista[] = {1,2,3,4,5,6,7,8,9,10};

    int res = iSum(lista, 10);

    printf("Resultado = %d",res);

    return 0;
}

int iSum(int list[], int size) {
    assert(size > 2);
    int oper = 0, cont = 0;

    for (int i = 0; i < size - 2; i++){
        for (int j = i + 1; j < size - 1; j++) {
            for (int k = j + 1; k < size; k++){
                oper++;
                if (list[k] == list[i] + list[j]) {
                    cont++; 
                }
            }
        }
    }

    printf("Número de operações:%d\n",oper);
    return cont;
}