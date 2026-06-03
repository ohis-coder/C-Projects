#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define ITERATIONS 10

int status_code = 99;
pthread_mutex_t lock;
pthread_cond_t whistle;

void *reader_logger(void *arg) {
  char *thread_name = (char *)arg;
  int i;

  for (i = 0; i < ITERATIONS; i++) {
    pthread_mutex_lock(&lock);

    if (thread_name[7] == 'A') {
      while (status_code != 0) {
        pthread_cond_wait(&whistle, &lock);
      }
      if (status_code == 0) {
        status_code = 99;
        printf("Thread A: Code 0 detected, Changing to %d\n\n", status_code);
      }
    } else {
      while (status_code != 99) {
        pthread_cond_wait(&whistle, &lock);
      }

      printf("Thread B: The Status Code is: %d so we are resetting now\n\n",
             status_code);
      status_code = 0;
    }
    pthread_cond_signal(&whistle);
    pthread_mutex_unlock(&lock);
    usleep(10000);
  }
  return NULL;
}

int main() {
  pthread_t thread1, thread2;

  if (pthread_mutex_init(&lock, NULL) != 0) {
    perror("Failed to initialize mutex");
    return 1;
  }

  if (pthread_cond_init(&whistle, NULL) != 0) {
    perror("Failed to initialize condition");
    return 1;
  }

  if (pthread_create(&thread1, NULL, reader_logger, "Thread A") != 0) {
    perror("Failed to create thread1");
    return 1;
  }

  if (pthread_create(&thread2, NULL, reader_logger, "Thread B") != 0) {
    perror("Failed to create thread2");
    return 1;
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&whistle);

  return 0;
}
