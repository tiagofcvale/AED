#include <stdio.h>
#include <assert.h>

int pgeo(int[], int);

int main(void) {
    int lista[] = {1,2,4,8,16,32,64,128,256,512};
    int res = pgeo(lista, 10);

    if (res == 1) {
        printf("Resultado = Não é progressão geométrica\n");
    } else {
        printf("Resultado = É progressão geométrica\n");
    }
}

int pgeo(int list[], int size) {
    assert(size>2);

    int r = list[1] / list[0];

    int oper = 0, res = 0;  
    for (int i = 2; i < size; i++) {
        oper++;
        if (list[i] != r*list[i-1]) {
            res = 1;
            break;
        }
    }
    printf("Número de Operações: %d\n",oper);

    return res;
}