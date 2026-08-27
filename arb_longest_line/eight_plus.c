#include <stdio.h>
#define MAXLINE 100
#define CHARMAX 20

int twenty_plus_sz(char s[], int limit);
void copy(char to[], char from[]);

int main() {
  int len;
  char line[MAXLINE];
  char longest[MAXLINE];

  int max = 20;
  while ((len = twenty_plus_sz(line, MAXLINE)) < 0) {
    //  transfer into longest as long as len > max
    //  pause the whole thing
    if (len > max) {
    }
  }

  return 0;
}

int twenty_plus_sz(char s[], int limit) {
  int c, i, j, sz;

  for (i = 0; i < limit - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
    // this is the main logic, so only when the len is > CHARMAX is when i
    // want..
    // ...to start adding to the char.. that means that it can join but once
    // there is a '\n' and i is not > CHARMAX discard.. so two ways we filter
    // before or after len return... well always guard contrapositive
    // instead i am using a sliding window, i to copy and j to track len
    for (j = 0; j < limit - 1; j++) {
      // hold
      // i shoould track the len w i
      if (i <= CHARMAX) {
        // sz = i;
      }
      s[j] = c;
      i++;
      j++;
    }
  }

  s[i] = '\0';
  s[j] = '\0';

  return i;
}

void copy(char to[], char from[]) {
  int i = 0;
  while ((to[i] = from[i]) != '\0') {
    i++;
  }
}
