#include <stdio.h>
#define MAX 10

// 0
int key[MAX] = {3, 8, 2, 1};
int next[MAX] = {3, 2, 0, -1};
int head = 1;
// 0

void print_linked_list() {
  int current = head;

  while (current != -1) {
    printf("%d ", key[current]);
    current = next[current];
  }
}
int main() {
  // delete(8);
  print_linked_list();
  return 0;
}
