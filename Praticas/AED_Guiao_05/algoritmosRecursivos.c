#include <stdio.h>
#include <assert.h>

int calls;

int T1(int);
int T2(int);
int T3(int);

int main(void) { 

    for (int n = 1; n <= 15; n++) {

        calls = 0;

        //int res = T1(n);

        //int res = T2(n);
        
        int res = T3(n);

        printf("\n\n===============\n");
        printf("N = %d\n",n);
        printf("resultado = %d\n",res);
        printf("Número de chamadas = %d\n",calls);
        printf("===============\n");
    }
}

int T1(int n) {
    assert(n > 0);
    calls++;

    if (n == 1) { return 1; }
    return (T1(n/2) + n);
}

int T2(int n) {
    assert(n > 0);
    calls++;

    if (n == 1) {return 1;}
    return (T2(n/2) + T2((n+1)/2));
}

int T3(int n) {
    assert(n > 0);
    calls++;

    if (n == 1) {return 1;} 
    else if (n%2 != 0) {return (T3(n/2) + T3((n+1)/2) + n);} 
    else {return (2*T3(n/2) + n);}
}
