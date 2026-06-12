#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERATIONS 3
#define NEW_LIST_LEN 6

void *read_upper(void *arg);
void *read_lower(void *arg);
void *run_processes(void);

typedef struct __attribute__((aligned(256))) {
  int upper_newlist[4] __attribute__((aligned(64)));
  int lower_newlist[4] __attribute__((aligned(64)));
  int upper[4];
  int lower[4];
} packet;

#endif
