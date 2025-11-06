//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira, Nov 2023, Nov 2024
//

// Complete the functions (marked by ...)
// so that they pass all tests.

#include "PersonSet.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definition of the structure
struct _PersonSet_ {
  int capacity;    // the current capacity of the array
  int size;        // the number of elements currently stored
  Person **array;  // points to an array of pointers to persons
};

#define INITIAL_CAPACITY 4

// You may add auxiliary definitions and declarations here, if you need to.

// Create a PersonSet.
PersonSet *PersonSetCreate() {
  // You must allocate space for the struct and for the array.
  // The array should be created with INITIAL_CAPACITY elements.
  // (The array will be reallocated if necessary, when elements are appended.)

  // COMPLETE ...
  
  PersonSet *ps = malloc(sizeof(PersonSet));

  if (ps==NULL) {return NULL;}

  ps->capacity = INITIAL_CAPACITY;
  ps->size = 0;
  ps->array = malloc(ps->capacity * sizeof(Person *));

  if (ps->array == NULL) {
    free(ps);
    return NULL;
  }

  return ps;
}

// Destroy PersonSet *pps
void PersonSetDestroy(PersonSet **pps) {
  assert(*pps != NULL);

  free((*pps)->array);
  free((*pps));
  *pps = NULL;
}

int PersonSetSize(const PersonSet *ps) { return ps->size; }

int PersonSetIsEmpty(const PersonSet *ps) { return ps->size == 0; }

void PersonSetPrint(const PersonSet *ps) {
  printf("{\n");
  for (int i = 0; i < ps->size; i++) {
    Person *p = ps->array[i];
    PersonPrintf(p, ";\n");
  }
  printf("}(size=%d, capacity=%d)\n", ps->size, ps->capacity);
}

// Find index in ps->array of person with given id.
// (INTERNAL function.)
static int search(const PersonSet *ps, int id) {
  // COMPLETE ...

  for (int i = 0; i < ps->size; i++) {
    Person *p = ps->array[i];
    if (PersonGetId(p) == id) {
      return i;
    }
  }

  return -1;
}

// Append person *p to *ps, without verifying presence.
// Use only when sure that *p is not contained in *ps!
// (INTERNAL function.)
static void append(PersonSet *ps, Person *p) {
  // MODIFY the function so that if the array is full,
  // it uses realloc to double the array capacity!

  // COMPLETE ...

  if (ps->size == ps->capacity) {
    //reallocate memory for the array
    Person **newPs =realloc(ps->array, ps->capacity * 2 * sizeof(Person *));
    if (newPs == NULL)  {
      perror("realloc failed"); // sou burro
      return;
    }
    ps->array = newPs;
  }

  ps->array[ps->size] = p;
  ps->size++;
}

// Add person *p to *ps.
// Do nothing if *ps already contains a person with the same id.
void PersonSetAdd(PersonSet *ps, Person *p) {
  // You may call the append function here!

  // COMPLETE ...
  int person_idx = search(ps,p->id);

  if (person_idx == -1) {
    append(ps,p);
  }

  return;
}

// Pop one person out of *ps.
Person *PersonSetPop(PersonSet *ps) {
  assert(!PersonSetIsEmpty(ps));
  // It is easiest to pop and return the person in the last position!

  // COMPLETE ...

  Person *p = ps->array[ps->size - 1];

  ps->size--;

  return p;
}

// Remove the person with given id from *ps, and return it.
// If no such person is found, return NULL and leave set untouched.
Person *PersonSetRemove(PersonSet *ps, int id) {
  // You may call search here!

  // COMPLETE ...

  int person_idx = search(ps,id);

  if (person_idx == -1) {
    return NULL;
  }

  Person *p = ps->array[person_idx];

  ps->array[person_idx] = PersonSetPop(ps);
  return p;
}

// Get the person with given id of *ps.
// return NULL if it is not in the set.
Person *PersonSetGet(const PersonSet *ps, int id) {
  // You may call search here!

  // COMPLETE ...

  int person_idx = search(ps, id);

  if (person_idx == -1) {
    return NULL;
  }
  
  Person *p = ps->array[person_idx];

  return p;
}

// Return true (!= 0) if set contains person wiht given id, false otherwise.
int PersonSetContains(const PersonSet *ps, int id) {
  return search(ps, id) >= 0;
}

// Return a NEW PersonSet with the union of *ps1 and *ps2.
// Return NULL if allocation fails.
// NOTE: memory is allocated.  Client must call PersonSetDestroy!
PersonSet *PersonSetUnion(const PersonSet *ps1, const PersonSet *ps2) {
  PersonSet *ps = PersonSetCreate();

  // COMPLETE ...
  for (int i = 0; i < ps1->size; i++) {
   Person *p = ps1->array[i];
   PersonSetAdd(ps,p);
  }

  for (int i = 0; i < ps2->size; i++) {
   Person *p = ps2->array[i];
   PersonSetAdd(ps,p);
  }

  return ps;
}

// Return a NEW PersonSet with the intersection of *ps1 and *ps2.
// Return NULL if allocation fails.
// NOTE: memory is allocated.  Client must call PersonSetDestroy!
PersonSet *PersonSetIntersection(const PersonSet *ps1, const PersonSet *ps2) {
  // COMPLETE ...
  PersonSet *ps = PersonSetCreate();

    for (int i = 0; i < ps1->size; i++) {
      Person *p1 = ps1->array[i];
      for (int j = 0; j < ps2->size; j++ ) {
        Person *p2 = ps2->array[j];
        if(p1->id == p2->id) {
          PersonSetAdd(ps,p1);
        }
      }
    }
  return ps;
}

// Return a NEW PersonSet with the set difference of *ps1 and *ps2.
// Return NULL if allocation fails.
// NOTE: memory is allocated.  Client must call PersonSetDestroy!
PersonSet *PersonSetDifference(const PersonSet *ps1, const PersonSet *ps2) {
  // COMPLETE ...
  PersonSet *ps = PersonSetCreate();

  for (int i = 0; i < ps1->size; i++) {
    Person *p1 = ps1->array[i];
    if (!PersonSetContains(ps2, p1->id)) {
      PersonSetAdd(ps, p1);
    }
  }

  return ps;
}

// Return true iff *ps1 is a subset of *ps2.
int PersonSetIsSubset(const PersonSet *ps1, const PersonSet *ps2) {
  // COMPLETE ...

  for (int i = 0; i < ps1->size; i++) {
    Person *p1 = ps1->array[i];

    if (!PersonSetContains(ps2, p1->id)) {
      return 0;
    }
  }

  return 1;
}

// Return true if the two sets contain exactly the same elements.
int PersonSetEquals(const PersonSet *ps1, const PersonSet *ps2) {

  // COMPLETE ...
  if (ps1->size != ps2->size) {
    return 0;
  }

  // Se ambos forem subsets um do outro, são iguais
  if (PersonSetIsSubset(ps1, ps2) && PersonSetIsSubset(ps2, ps1)) {
    return 1;
  }

  return 0;

}
