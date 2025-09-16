#include <stdio.h>

int main(void) {
    int my_num;
    char my_char;
    printf("Type a Number AND a character then press enter:  \n");
    scanf("%d %c", &my_num, &my_char);
    printf("Your number is : %d and your character is: %c\n",my_num, my_char);      //%d para ints e %c para caracteres

    char first_name[30];            //Assumir que o nome é uma array de até 30 caracteres
    printf("Enter your name: \n");
    scanf("%s", first_name);       //%s para "strings"
    printf("Hello %s\n",first_name);
}