#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <sys/signal.h>
#include <unistd.h>

#define FAILED -1
#define MAXJOBS 9
#define SWAP(type, x, y)                                                       \
  do {                                                                         \
    type temp = x;                                                             \
    x = y;                                                                     \
    y = temp;                                                                  \
  } while (0)

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
    return -1;
  }
  return c - '0';
}

job_profile get_job_and_completion_time() {
  job_profile job;
  char buffer[8];

  printf("\nWhat is the name of this job?(< 8 chars bro)\n");

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
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

/*
void sort_array_descending(job_profile arr[], int size) {
  int start, scan;

  for (start = 0; start < size - 1; start++) {
    for (scan = start + 1; scan < size - 1; scan++) {
      if (arr[scan].completion_time > arr[start].completion_time) {
        // arr[scan], arr[start] = arr[start], arr[scan]; lmao i thought this
        // was python leetcode smh
        SWAP(job_profile, arr[start], arr[scan]);
      }
    }
  }
}
*/

int main() {
  job_profile arr[MAXJOBS];
  int i, j, k;
  int l = 0;
  int jc = get_total_job_count();

  for (i = 0; i < jc; i++) {
    job_profile job = get_job_and_completion_time();

    arr[i] = job;
  }

  pid_t worker_pid;

  worker_pid = fork();

  if (worker_pid == 0) {
    setvbuf(stdout, NULL, _IONBF, 0);
    while (1) {
      sleep(1);
      printf("Child Process Working on task..\n");
    }
  } else {
    printf("Monitoring Child Process %d\n", worker_pid);
    while (l + 1 < jc) {
      job_profile current_job = arr[l];
      job_profile next_job = arr[l + 1];
      if (l + 1 < jc &&
          current_job.completion_time > next_job.completion_time) {
        sleep(2);
        kill(worker_pid, SIGSTOP);
        printf("Shorter Job Found, Switching Task Now\n");
        sleep(1);
        l++;
        kill(worker_pid, SIGCONT);
      } else {
        l++;
      }
    }
  }
  kill(worker_pid, SIGKILL);
  printf("All Tasks Successfully Handled!\n");
  return 0;
}
