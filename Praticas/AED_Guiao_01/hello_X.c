#include <stdio.h>

int main(void) {
    char first[30];
    char last[30];
    printf("Introduza o seu nome: ");
    scanf("%s %s",first,last);
    printf("Hello %s %s!\n", first,last);
    return 0;
}
