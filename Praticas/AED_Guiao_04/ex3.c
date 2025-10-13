#include <stdio.h>
#include <assert.h>

int iSum(int[], int);

int main(void) {
    int lista[] = {1,2,3,4,5,6,7,8,9,10};

}

int iSum(int list[], int size) {
    assert(list >2);
    int oper = 0, cont = 0;

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++){
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