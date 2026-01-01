/* test_deque.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deque.h"

static int* mkint(int v) {
    int* p = (int*)malloc(sizeof(int));
    assert(p != NULL);
    *p = v;
    return p;
}

static void print_ptr_int(const char* label, void* p) {
    if (p == NULL) {
        printf("%sNULL\n", label);
    } else {
        printf("%s%d\n", label, *(int*)p);
    }
}

int main(void) {
    printf("=== TESTE Deque ===\n");

    Deque* d = DequeCreate(10);
    assert(d != NULL);

    printf("\n[Estado inicial]\n");
    printf("Size: %d\n", DequeSize(d));
    printf("IsEmpty: %d\n", DequeIsEmpty(d));
    printf("IsFull: %d\n", DequeIsFull(d));
    print_ptr_int("Peek front: ", DequePeek_at_front(d));
    print_ptr_int("Peek rear : ", DequePeek_at_rear(d));

    printf("\n[Adicionar atrás: 10, 20, 30]\n");
    assert(DequeAdd_at_rear(d, mkint(10)) == 1);
    assert(DequeAdd_at_rear(d, mkint(20)) == 1);
    assert(DequeAdd_at_rear(d, mkint(30)) == 1);
    printf("Size: %d\n", DequeSize(d));
    print_ptr_int("Peek front: ", DequePeek_at_front(d)); // 10
    print_ptr_int("Peek rear : ", DequePeek_at_rear(d));  // 30

    printf("\n[Adicionar à frente: 5, 2]\n");
    assert(DequeAdd_at_front(d, mkint(5)) == 1);
    assert(DequeAdd_at_front(d, mkint(2)) == 1);
    printf("Size: %d\n", DequeSize(d));
    print_ptr_int("Peek front: ", DequePeek_at_front(d)); // 2
    print_ptr_int("Peek rear : ", DequePeek_at_rear(d));  // 30

    printf("\n[Remover à frente x2]\n");
    void* r = DequeRemove_at_front(d);  // 2
    print_ptr_int("Removed front: ", r);
    free(r);

    r = DequeRemove_at_front(d);        // 5
    print_ptr_int("Removed front: ", r);
    free(r);

    printf("Size: %d\n", DequeSize(d));
    print_ptr_int("Peek front: ", DequePeek_at_front(d)); // 10
    print_ptr_int("Peek rear : ", DequePeek_at_rear(d));  // 30

    printf("\n[Remover atrás x1]\n");
    r = DequeRemove_at_rear(d);         // 30
    print_ptr_int("Removed rear: ", r);
    free(r);

    printf("Size: %d\n", DequeSize(d));
    print_ptr_int("Peek front: ", DequePeek_at_front(d)); // 10
    print_ptr_int("Peek rear : ", DequePeek_at_rear(d));  // 20

    printf("\n[Encher até ficar full]\n");
    while (!DequeIsFull(d)) {
        assert(DequeAdd_at_rear(d, mkint(99)) == 1);
    }
    printf("Size: %d\n", DequeSize(d));
    printf("IsFull: %d\n", DequeIsFull(d));
    printf("Tentar add (deve falhar): %d\n", DequeAdd_at_rear(d, mkint(123))); // cuidado: isto aloca

    /* Se a última inserção falhar, libertar o 123 para não dar leak */
    /* Como não sabemos se falhou (esperamos que sim), vamos ser seguros:
       se a deque estava full, a função devolve 0 e o ponteiro não entrou. */
    /* Para garantir, fazemos assim: */
    int* leak_guard = mkint(1234);
    int ok = DequeAdd_at_front(d, leak_guard);
    if (ok == 0) free(leak_guard);

    printf("\n[Esvaziar removendo tudo (front) e libertar memória]\n");
    while (!DequeIsEmpty(d)) {
        r = DequeRemove_at_front(d);
        free(r);
    }
    printf("Size: %d\n", DequeSize(d));
    printf("IsEmpty: %d\n", DequeIsEmpty(d));

    printf("\n[Teste Clear + estado]\n");
    assert(DequeAdd_at_rear(d, mkint(1)) == 1);
    assert(DequeAdd_at_rear(d, mkint(2)) == 1);
    printf("Size antes do clear: %d\n", DequeSize(d));

    /* ATENÇÃO: DequeClear só limpa a estrutura; se guardaste ponteiros,
       és tu que tens de os libertar antes. Vamos removê-los primeiro. */
    free(DequeRemove_at_front(d));
    free(DequeRemove_at_front(d));

    DequeClear(d);
    printf("Size depois do clear: %d\n", DequeSize(d));
    printf("IsEmpty: %d\n", DequeIsEmpty(d));
    print_ptr_int("Peek front: ", DequePeek_at_front(d));
    print_ptr_int("Peek rear : ", DequePeek_at_rear(d));

    printf("\n[Destroy]\n");
    DequeDestroy(&d);
    printf("Deque pointer após destroy: %p\n", (void*)d);

    printf("\n=== OK ===\n");
    return 0;
}
