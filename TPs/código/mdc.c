#include <stdio.h>

int mdc(int a, int b);

int main(int argc, char** argv) {
    printf("%d\n",mdc(195,235));
}

int mdc(int a, int b) {
    if (b == 0) return a;
    return mdc(b, a % b);
}