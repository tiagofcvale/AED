#include <string.h>
#include <stdio.h>

char *toLowerCase(char *str);
int checkIfUpperAlfa(char c);
int checkIfLowerAlfa(char c);
void sortString(char *str);

int main() {

    char str1[20] = "aBcDeF12";
    char str2[40] = "Strings em C são fixes";
    int n_alfa = 0;
    for (int i = 0; i < strlen(str1); i++) {
        int bool1 = checkIfLowerAlfa(str1[i]);
        int bool2 = checkIfUpperAlfa(str1[i]);
        if ((bool1 + bool2) >= 1){
            n_alfa++;
        } 
    }
    printf("Nº de caracteres de string1 que são letras: %d \n",n_alfa);

    int n_upperAlfa = 0;
    for (int i = 0; i < strlen(str2); i++) {
        int bool = checkIfUpperAlfa(str2[i]);
        if (bool == 1) {
            n_upperAlfa++;
        }
    }
    printf("Nº de caracteres de string2 que são letras maiúsculas: %d \n",n_upperAlfa);

    char *strL1 = toLowerCase(str1);
    char *strL2 = toLowerCase(str2);

    printf("String1 para minúsculas: %s \n",strL1);
    printf("String2 para minúsculas: %s \n",strL2);

    char strC2[25] = "";
    strcpy(strC2, str2);
    printf("Cópia da string2 + string2 = %s \n",strcat(strC2, str2));

    int comp = strcmp(str1, str2);
    if (comp == 0) {
        printf("Strings iguais \n");
    } else {
            printf("Strings diferentes \n");
    }

    sortString(str1);
    printf("str1 para ordem alfabética: %s\n", str1);

    sortString(str2);
    printf("str2 para ordem alfabética: %s\n", str2);
}

char *toLowerCase(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        int bool3 = checkIfUpperAlfa(str[i]);
        if (bool3 == 1){
            str[i] = str[i] + 32;
        }
    }
    return str;
}

int checkIfUpperAlfa(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    return 0;
}

int checkIfLowerAlfa(char c) {
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    return 0;
}


void sortString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (str[j] > str[j + 1]) {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}