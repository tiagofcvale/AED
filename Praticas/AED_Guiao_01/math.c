#include <stdio.h>
#include <math.h>

int main(void) {
    int linhas;

    printf("Quantas linhas tem a tabela? 0<linhas<9: ");
    scanf("%d", &linhas);

    if (linhas <= 0 || linhas >= 9) {
        return 0;
    }

    printf("num....num².......sqrt\n");

    for (int i = 1; i <= linhas; i++) {
        printf("%d ..... %.2f .. %.2f\n", i, pow(i, 2), sqrt(i));
    }

    return 0;
}
