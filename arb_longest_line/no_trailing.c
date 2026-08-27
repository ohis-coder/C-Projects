#include <stdio.h>
#define MAXCHARS 10000

void get_clean_char();
int main() {
  get_clean_char();
  return 0;
}

void get_clean_char() {
  int c;
  int prev = '\n';

  while ((c = getchar()) != EOF) {
    if (c == '\n' && prev == '\n') {
      continue;
    }
    if (c == ' ' && prev == ' ') {
      continue;
    }
    if (c == '\t') {
      putchar(' ');
      continue;
    }
    putchar(c);
    prev = c;
  }

  return;
}
