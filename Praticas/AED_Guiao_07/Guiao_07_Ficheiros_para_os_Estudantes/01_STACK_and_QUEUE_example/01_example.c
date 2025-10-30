//
// TO DO : desenvolva um algoritmo para verificar se um numero inteiro positivo
//         é uma capicua
//         Exemplos: 12321 e uma capiacua, mas 123456 nao e
//         USE uma PILHA DE INTEIROS (STACK) e uma FILA DE INTEIROS (QUEUE)
//
// TO DO : design an algorithm to check if the digits of a positive decimal
//         integer number constitue a palindrome
//         Examples: 12321 is a palindrome, but 123456 is not
//         USE a STACK of integers and a QUEUE of integers
//

#include <stdio.h>
#include <stdlib.h>

#include "IntegersQueue.h"
#include "IntegersStack.h"

int isPalindrome(int number) {
    Queue* queue = QueueCreate(100); // maxSize = 100
    Stack* stack = StackCreate(100);

    // Extract numbers
    while (number > 0) {
        int digit = number % 10;

        QueueEnqueue(queue, digit);
        StackPush(stack, digit);
        
        number /= 10;
    }

    // Compare elements of stack and queue
    while(!QueueIsEmpty(queue) || !StackIsEmpty(stack)) {
        if (QueueDequeue(queue) != StackPop(stack)) {

            QueueDestroy(&queue);
            StackDestroy(&stack);
            return 0; // Not palindrome 
        }
    }

    QueueDestroy(&queue);
    StackDestroy(&stack);
    return 1; // Palindrome
}
int main(int argc, char* argv[]) { 
    int number;
    
    if (argc != 2) {
        printf("Usage: %s <number>\n",argv[0]);
        return 1;
    }

    number = atoi(argv[1]);

    if (isPalindrome(number)) {
        printf("%d é capicua\n",number);
    } else {
        printf("%d não é capicua\n",number);
    }
    return 0;
}
