#include <stdio.h>
#define MAXLINE 1000
#define CHARMAX 20

int getline_kr(char s[], int limit);
void copy(char to[], char from[]);

int main() {
  int len;
  char line[MAXLINE];
  // char newline[MAXLINE];

  while ((len = getline_kr(line, MAXLINE)) > 0) {
    if (len > CHARMAX) {
      // copy
      // copy(line, newline);
      printf("%s", line);
    }
    // printf("%s", newline);
  }
  return 0;
}

int getline_kr(char s[], int limit) {
  int c, i;

  for (i = 0; i < limit - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
    s[i] = c;
  }
  if (c == '\n') {
    s[i] = c;
    i++;
  }

  s[i] = '\0';

  return i;
}

void copy(char to[], char from[]) {
  int i = 0;
  while ((to[i] = from[i]) != '\0') {
    i++;
  }
}
