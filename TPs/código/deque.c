#include <assert.h>
#include <stdlib.h>
#include "deque.h"

struct _PointersDeque {
    int max_size;
    int cur_size;
    int head;
    int tail;
    void** data;
};

static int increment_index(const Deque* d, int i) {
    return (i + 1 < d->max_size) ? (i + 1) : 0;
}

static int decrement_index(const Deque* d, int i) {
    return (i - 1 >= 0) ? (i - 1) : (d->max_size - 1);
}

Deque* DequeCreate(int size) {
    assert(size >= 10 && size <= 10000);

    Deque* d = malloc(sizeof *d);
    if (d == NULL) return NULL;

    d->max_size = size;
    d->cur_size = 0;

    d->head = 0;
    d->tail = -1;

    d->data = malloc(size * sizeof(void*));
    if (d->data == NULL) {
        free(d);
        return NULL;
    }

    return d;
}

void DequeDestroy(Deque** d) {
    if (d == NULL || *d == NULL) return;
    free((*d)->data);
    free(*d);
    *d = NULL;
}

void DequeClear(Deque* d) {
    assert(d != NULL);
    d->cur_size = 0;
    d->head = 0;
    d->tail = -1;
}

int DequeSize(const Deque* d) {
    assert(d != NULL);
    return d->cur_size;
}

int DequeIsEmpty(const Deque* d) {
    assert(d != NULL);
    return d->cur_size == 0;
}

int DequeIsFull(const Deque* d) {
    assert(d != NULL);
    return d->cur_size == d->max_size;
}

int DequeAdd_at_front(Deque* d, void* p) {
    assert(d != NULL);
    if (DequeIsFull(d)) return 0;

    if (DequeIsEmpty(d)) {
        d->head = 0;
        d->tail = 0;
    } else {
        d->head = decrement_index(d, d->head);
    }

    d->data[d->head] = p;
    d->cur_size++;
    return 1;
}

int DequeAdd_at_rear(Deque* d, void* p) {
    assert(d != NULL);
    if (DequeIsFull(d)) return 0;

    if (DequeIsEmpty(d)) {
        d->head = 0;
        d->tail = 0;
    } else {
        d->tail = increment_index(d, d->tail);
    }

    d->data[d->tail] = p;
    d->cur_size++;
    return 1;
}

void* DequePeek_at_front(const Deque* d) {
    assert(d != NULL);
    if (DequeIsEmpty(d)) return NULL;
    return d->data[d->head];
}

void* DequePeek_at_rear(const Deque* d) {
    assert(d != NULL);
    if (DequeIsEmpty(d)) return NULL;
    return d->data[d->tail];
}

void* DequeRemove_at_front(Deque* d) {
    assert(d != NULL);
    if (DequeIsEmpty(d)) return NULL;

    void* removed = d->data[d->head];

    if (d->cur_size == 1) {
        d->cur_size = 0;
        d->head = 0;
        d->tail = -1;
        return removed;
    }

    d->head = increment_index(d, d->head);
    d->cur_size--;
    return removed;
}

void* DequeRemove_at_rear(Deque* d) {
    assert(d != NULL);
    if (DequeIsEmpty(d)) return NULL;

    void* removed = d->data[d->tail];

    if (d->cur_size == 1) {
        d->cur_size = 0;
        d->head = 0;
        d->tail = -1;
        return removed;
    }

    d->tail = decrement_index(d, d->tail);
    d->cur_size--;
    return removed;
}