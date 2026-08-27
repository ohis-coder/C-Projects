#include <stddef.h>
#include <stdio.h>

#define MAXLINE 500
#define MAXINPUT 20

int get_arb_line(char s[], int limit);

int get_arb_line(char s[], int limit) {
  int c, i;

  for (i = 0; i < limit - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
    s[i] = c;
    if (i > MAXINPUT) {
      strncpy(longest, s, MAXLINE - 1);
      return i;
    }
  }

  return i;
}

int main() {

  char line[MAXLINE];
  char longest[MAXLINE];

  get_arb_line(line, MAXLINE);
  return 0;
}
