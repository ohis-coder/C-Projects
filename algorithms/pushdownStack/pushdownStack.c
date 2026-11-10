#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// this key is faulty on my keyboard so yea: 0 )

struct node {
  int key;
  struct node *next;
};

struct node *head, *z, *t;

void stackinit() {
  head = (struct node *)malloc(sizeof(*head));
  z = (struct node *)malloc(sizeof(*z));

  head->next = z;
  head->key = 0;
  z->next = z;

  return;
}

void push(int v) {
  t = (struct node *)malloc(sizeof(*t));
  t->key = v;
  t->next = head->next;
  head->next = t;

  return;
}

int pop() {
  int x;
  t = head->next;
  head->next = t->next;
  x = t->key;
  free(t);

  return x;
}

int stackempty() { return head->next == z; }

int main() {
  struct node *head, *z, *t;

  stackinit();
  push(5);
  int p = pop();

  printf("%d", p);
}
