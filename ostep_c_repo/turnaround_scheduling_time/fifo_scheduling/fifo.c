#include <ctype.h>
#include <stdio.h>

// this key is faulty on my keyboard so yea: 0 )

int getnum() {
  int i, c;

  printf("Input Task Completion Time (< 9 secs bro)\n");

  c = getchar();

  int next_char = getchar();

  if (next_char != '\n' && next_char != EOF) {
    printf("Single digits only bro\n");
    return -1;
  }
  if (c == EOF || !isdigit(c) || c > '9') {
    printf("Input a valid number between 0 - 9 dawg\n");
    return -1;
  }
  return c - '0';
}

int main() {
  int num = getnum();
  printf("%d\n", num);
  return 0;
}
