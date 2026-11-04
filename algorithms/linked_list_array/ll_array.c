#include <stdio.h>
#define MAX 10

int main() {
  int key[MAX] = {3, 8, 2, 1};
  int next[MAX] = {3, 2, 0, -1};
  // int idx[MAX] = {0, 1, 2, 3};

  int head = 0;
  int x = 4;
  int i;

  for (i = head; i < 3; i++) {
    printf("%d", key[next[i]]);
  }
  return 0;
}
