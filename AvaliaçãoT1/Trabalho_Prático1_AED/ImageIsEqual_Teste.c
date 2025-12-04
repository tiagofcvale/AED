#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "imageRGB.h"
#include "instrumentation.h"

int main() {

    ImageInit();

    printf("\nTESTE\n");

    // utiliza a função de carregar imagens (do ficheiro ImageRGB.c para carregar as imagens da pasta)
    Image img1 = ImageLoadPPM("img_case1.ppm"); //abre uma imagem 10*10 em que todos os pixeis são brancos
    Image img2 = ImageLoadPPM("img_case2.ppm"); //abre uma imagem 10*10 em que apenas o primeiro pixel é diferente da imagem de cima
    Image img3 = ImageLoadPPM("img_case3.ppm"); //abre uma imagem em que todos os pixeis são brancos, mas desta vez 100*100
    Image img4 = ImageLoadPPM("img_case4.ppm"); //abre uma imagem 100*100 em que apenas o ultimo pixel é preto
    Image img5 = ImageLoadPPM("img_case5.ppm"); // abre uma imagem de 100*90 totalmente branca

    if (!img1 || !img2 || !img3 || !img4 || !img5) {
        fprintf(stderr, "Erro a carregar uma das imagens\n");
        return 1;
    }

    
    InstrReset();
    int r1 = ImageIsEqual(img1, img1);
    printf("Caso 1 (10x10 iguais): Resultado=%d | Comparações=%lu\n", r1, InstrCount[0]); //imagens iguais com pixeis iguais.
    //vai comparar todos os pixeis. Isto será o pior caso O(W x H)

    InstrReset();
    int r2 = ImageIsEqual(img1, img2);
    printf("Caso 2 (10x10 1º pixel diferente): Resultado=%d | Comparações=%lu\n", r2, InstrCount[0]); //imagens com dimensões iguais, mas o primeiro pixel é diferente
    //logo, isto corresponde ao melhor caso O(1)

    InstrReset();
    int r3 = ImageIsEqual(img3, img3);
    printf("Caso 3 (100x100 iguais): Resultado=%d | Comparações=%lu\n", r3, InstrCount[0]); //imagens com dimensões iguais
    //pior caso, O(W x H)

    InstrReset();
    int r4 = ImageIsEqual(img3, img4);
    printf("Caso 4 (100x100 último pixel diferente): Resultado=%d | Comparações=%lu\n", r4, InstrCount[0]); //imagens com dimensões iguais, mas o último pixel é diferente
    //logo, vai resultar no pior caso O(W x H) pois continua a ter que fazer todas as comparações

    InstrReset();
    int r5 = ImageIsEqual(img4, img5);
    printf("Caso 5 (100x90 dimensões diferentes): Resultado=%d | Comparações=%lu\n", r5, InstrCount[0]);//imagens com dimensões diferentes
    //não faz nenhuma comparação pois as imagens nunca podem ser iguais. Melhor caso O(1)

    ImageDestroy(&img1);
    ImageDestroy(&img2);
    ImageDestroy(&img3);
    ImageDestroy(&img4);
    ImageDestroy(&img5);

    

    return 0;
}
