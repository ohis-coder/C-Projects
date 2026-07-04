#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FAILED -1
// this key is faulty on my keyboard so yea: 0 )

typedef struct {
  char job[8];
  int completion_time;
  char padding[4];
} __attribute__((aligned(16))) job_profile;

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

job_profile get_job_and_completion_time() {
  job_profile job;
  char buffer[8];

  printf("\nWhat is the name of this job?(< 8 chars bro)\n");

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    job.completion_time = FAILED;
    strncpy(job.job, buffer, sizeof(job.job) - 1);

    return job;
  }

  int len = 0;
  while (buffer[len] != '\0') {
    len++;
  }

  if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n') {
    printf("\nJob name was too damn long bro! Max 7 chars.\n");
    clear_stdin();
    job.completion_time = FAILED;
    strncpy(job.job, buffer, sizeof(job.job) - 1);

    return job;
  }

  printf("\nInput Task Completion Time (< 9 secs bro)\n");

  int job_time = getnum();
  job.completion_time = job_time;
  strncpy(job.job, buffer, sizeof(job.job) - 1);

  // clear_stdin();
  return job;
}

int get_total_job_count() {

  printf("\nHow many jobs are you processing today fine sir?\n(< 9 jobs bro, i "
         "dont get paid enough for this)\n");

  int job_count = getnum();

  return job_count;
}

int main() {
  int i, j;
  int jc = get_total_job_count();

  for (i = 0; i < jc; i++) {
    job_profile job = get_job_and_completion_time();

    printf("Job name: %s", job.job);
    printf("Job Completion Time: %d second(s)\n", job.completion_time);
  }

  return 0;
}
