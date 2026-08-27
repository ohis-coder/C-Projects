#include <stdio.h>

#define MAXLINE 20

int get_arb_line(char s[], int limit);
void copy(char a[], char b[]);

int main() {

  int len;
  int max;
  char line[MAXLINE];
  char longest[MAXLINE];

  max = 0;

  while ((len = get_arb_line(line, MAXLINE)) > 0) {
    printf("Line length read: %d\n", len);

    if (len > max) {
      max = len;
      copy(longest, line);
    }
  }

  if (max > 0) {
    printf("\n--- LONGEST LINE SUMMARY ---\n");
    printf("True total length: %d characters\n", max);
    printf("Text kept (up to buffer size): %s\n", longest);
  }
  return 0;
}

int get_arb_line(char s[], int limit) {
  int c, i;

  i = 0;

  while ((c = getchar()) != EOF && c != '\n') {
    if (i < limit - 1) {
      s[i] = c;
    }
    i++;
  }
  if (i < limit) {
    s[i] = '\0';
  } else {
    s[limit - 1] = 0;
  }
  return i;
}

void copy(char to[], char from[]) {
  int i = 0;
  while ((to[i] = from[i]) != '\0') {
    i++;
  }
}
