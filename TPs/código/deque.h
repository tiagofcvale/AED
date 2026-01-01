typedef struct _PointersDeque Deque;

Deque* DequeCreate(int size);
void   DequeDestroy(Deque** d);

void   DequeClear(Deque* d);

int    DequeSize(const Deque* d);
int    DequeIsEmpty(const Deque* d);
int    DequeIsFull(const Deque* d);

int    DequeAdd_at_front(Deque* d, void* p);
int    DequeAdd_at_rear (Deque* d, void* p);

void*  DequeRemove_at_front(Deque* d);
void*  DequeRemove_at_rear (Deque* d);

void*  DequePeek_at_front(const Deque* d);
void*  DequePeek_at_rear (const Deque* d);
