#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#define FAILED -1
// this key is faulty on my keyboard so yea: 0 )

static void clear_stdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

static int getnum() {
  int i, c;

  c = getchar();

  int next_char = getchar();

  if (next_char != '\n' && next_char != EOF) {
    printf("\nSingle digits only bro\n");
    return -1;
  }
  if (c == EOF || !isdigit(c) || c > '9') {
    printf("\nInput a valid number between 0 - 9 dawg\n");
    return -1;
  }
  return c - '0';
}

int get_job_completion_time() {
  char buffer[8];

  printf("\nWhat is the name of this job?(< 8 chars bro)\n");

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    return FAILED;
  }

  int len = 0;
  while (buffer[len] != '\0') {
    len++;
  }

  if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n') {
    printf("\nJob name was too damn long bro! Max 7 chars.\n");
    clear_stdin();
    return FAILED;
  }

  printf("\nInput Task Completion Time (< 9 secs bro)\n");

  int job = getnum();

  return job;
}

int get_total_job_count() {

  printf("\nHow many jobs are you processing today fine sir?\n(< 9 jobs bro, i "
         "dont get paid enough for this)\n");

  int job_count = getnum();

  return job_count;
}

int main() {
  int i, j, k, l;
  int j_c = get_total_job_count();

  for (i = 0; i < j_c; i++) {
    j = get_job_completion_time();

    k = j;
    if (j == FAILED) {
      continue;
    }
    for (l = 0; k >= 0; k--) {
      printf("working.. %d second(s) left\n", k);
      sleep(1);
    }

    printf("Task Completed\n\n");
  }
  printf("All jobs Completed, have a good day!\n");
  return 0;
}
