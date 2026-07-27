#include <stdio.h>
#include <stdlib.h>

// this key is faulty on my keyboard so yea: 0 )

struct myDict {
  char *name;
  int balance;
  char padding[4]; // just for flex
};

struct context;

struct ops {
  void (*grow)(struct context *self);
};

struct context {
  struct myDict **entries;
  struct ops operations;
  int capacity; // the projected size
  int size;     // hardcoded size of the array of structs
};

void increase_size(struct context *self);

void increase_size(struct context *self) {
  if (self->size < self->capacity) {
    return;
  }
  self->capacity *= 2;
  struct myDict **new_entries =
      realloc(self->entries, self->capacity * sizeof(struct myDict *));

  if (new_entries == NULL) {
    return;
  }
  self->entries = new_entries;
}

int main() {
  struct context *p = malloc(sizeof(struct context));
  if (p == NULL)
    return 0;

  // initialize everything
  p->size = 0;
  p->capacity = 10;
  p->entries = malloc(p->capacity * sizeof(struct myDict *));
  p->operations.grow = increase_size;

  printf("Current size: %d, capacity: %d\n", p->size, p->capacity);

  // add some entries
  for (int i = 0; i < 15; i++) {
    if (p->size >= p->capacity) {
      p->operations.grow(p);
    }

    // add an entry (simplified)
    p->entries[p->size] = malloc(sizeof(struct myDict));
    p->entries[p->size]->name = "ohimai";
    p->entries[p->size]->balance = i * 100;
    p->size++;
  }

  printf("After adding: size = %d, capacity = %d\n", p->size, p->capacity);
  printf("The current size of your dictionary is %d\n", p->size);

  // let us print something in it
  printf(
      "The Information in the Second dictionary is, Name: %s and Balance: %d\n",
      p->entries[1]->name, p->entries[1]->balance);
  printf(
      "The Information in the Third dictionary is, Name: %s and Balance: %d\n",
      p->entries[2]->name, p->entries[2]->balance);

  // 1. Free each individual myDict entry
  for (int i = 0; i < p->size; i++) {
    free(p->entries[i]); // Free the struct
  }

  // 2. Free the array of pointers
  free(p->entries);

  // 3. Free the context struct
  free(p);
  return 0;
}
