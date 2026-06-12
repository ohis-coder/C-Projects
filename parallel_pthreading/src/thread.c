#include "../include/thread.h"

void *read_upper(void *arg) {
  packet *p = (packet *)arg;
  for (int i = 0; i < ITERATIONS; i++) {
    p->upper_newlist[i] = p->upper[i];
  }
  return NULL;
}

void *read_lower(void *arg) {
  packet *p = (packet *)arg;
  for (int i = 0; i < ITERATIONS; i++) {
    p->lower_newlist[i] = p->lower[i];
  }
  return NULL;
}

void *run_processes(void) {
  packet *p = (packet *)malloc(sizeof(packet));
  int *newlist = malloc(6 * sizeof(int));

  memcpy(p->lower, (int[]){1, 4, 5}, 3 * sizeof(int));
  memcpy(p->upper, (int[]){0, 2, 3}, 3 * sizeof(int));

  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, read_upper, p) != 0) {
    perror("Failed to create Thread1");
  }

  if (pthread_create(&thread2, NULL, read_lower, p) != 0) {
    perror("Failed to create Thread1");
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  memcpy(newlist, p->upper_newlist, 3 * sizeof(int));
  memcpy(&newlist[3], p->lower_newlist, 3 * sizeof(int));

  for (int i = 0; i < NEW_LIST_LEN; i++) {
    printf("[%d]", newlist[i]);
  }

  printf("\n");

  free(p);
  free(newlist);

  return NULL;
}
