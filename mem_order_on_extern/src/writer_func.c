#include "../include/header.h"
#include <pthread.h>
#include <stdatomic.h>

// this key is faulty on my keyboard so yea: 0 )

atomic_int data = 0;

void *dumb_increase_one(void *arg) {
  (void)arg;
  int i;
  for (i = 0; i <= 5; i++) {
    // pthread_mutex_lock(&lock);
    atomic_fetch_add(&data, 1);
    // pthread_mutex_unlock(&lock);
  }
  return NULL;
}

/*
int dumb_increase_two(int data) {
        for (i = 0; i <= 5; i++) {
                data++;
                return data;
        }
}
*/

int run(void) {
  pthread_t t1, t2;

  if (pthread_create(&t1, NULL, dumb_increase_one, &data) != 0) {
    perror("Failed to create t1");
  }

  if (pthread_create(&t2, NULL, dumb_increase_one, &data) != 0) {
    perror("Failed to create t2");
  }

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  return atomic_load(&data);
}
