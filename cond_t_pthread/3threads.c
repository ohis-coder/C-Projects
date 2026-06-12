#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define ITERATIONS 3

int id1 = 0;
int id2 = 1;
int id3 = 2;
int shared_count = 0;

pthread_mutex_t lock;
pthread_cond_t whistle;

void *three_step(void *arg) {
  int *id = (int *)arg;

  for (int i = 0; i < ITERATIONS; i++) {
    pthread_mutex_lock(&lock);

    while (shared_count != *id) {
      pthread_cond_wait(&whistle, &lock);
    }
    if (*id == 0) {
      shared_count = 1;
      printf("Thread A: running, set count as %d\n", shared_count);
    } else if (*id == 1) {
      shared_count = 2;
      printf("Thread B: running, set count as %d\n", shared_count);
    } else {
      shared_count = 0;
      printf("Thread C: running, set count as %d\n", shared_count);
    }

    pthread_cond_broadcast(&whistle);
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main() {

  pthread_t thread1, thread2, thread3;

  if (pthread_mutex_init(&lock, NULL) != 0) {
    perror("Mutex Init Failed");
    return 1;
  }

  if (pthread_cond_init(&whistle, NULL) != 0) {
    perror("Condition Init Failed");
    return 1;
  }

  if (pthread_create(&thread1, NULL, three_step, &id1) != 0) {
    perror("Failed to Create Thread 1");
    return 1;
  }

  if (pthread_create(&thread2, NULL, three_step, &id2) != 0) {
    perror("Failed to Create Thread 2");
    return 1;
  }

  if (pthread_create(&thread3, NULL, three_step, &id3) != 0) {
    perror("Failed to Create Thread 3");
    return 1;
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&whistle);

  return 0;
}
