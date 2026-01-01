#include <stdio.h>

void selectionSort(int a[], int n);
void swap(int *a, int *b);

int main(int argc, char** argv) {
    int a[] = {5,6,7,4,3,6,7,4,5};
    int n = 9;
    printf("list before sort: \n");
    for(int i = 0; i < n; i++) {
        printf("%d ",a[i]);
    }
    printf("\n");
    selectionSort(a,n);
    
    printf("list after sort: \n");
    for(int j = 0; j < n; j++) {
        printf("%d ",a[j]);
    }
    printf("\n");
}

void selectionSort(int a[], int n) {
    for(int left = 0, right = n-1; left < right; right--, left++) {
        int indMax = right;
        int indMin = left;
        
        for (int i = 1; i <= right; i++) {
            if(a[i] >= a[indMax]) indMax = i;
            if(a[i] < a[indMin]) indMin = i;
        }
        if(indMax != right) swap(&a[indMax], &a[right]);

        if (indMax == left) indMax = indMin;

        if(indMin != left) swap(&a[indMin], &a[left]);
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
