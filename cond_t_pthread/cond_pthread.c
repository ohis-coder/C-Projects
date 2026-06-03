#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define ITERATION 10

int num = 0;
pthread_mutex_t lock;
pthread_cond_t ready;

void *count_up(void *arg) {
  char *thread_name = (char *)arg;

  for (int i = 0; i < ITERATION; i++) {
    pthread_mutex_lock(&lock);

    if (thread_name[7] == 'A') {
      while ((num & 1) != 0) {
        pthread_cond_wait(&ready, &lock);
      }
    } else {
      while ((num & 1) == 0) {
        pthread_cond_wait(&ready, &lock);
      }
    }
    num++;
    pthread_cond_signal(&ready);
    pthread_mutex_unlock(&lock);
    printf("Done increasing %s\n", thread_name);
  }
  return NULL;
}

// printf("Done increasing %s\n", thread_name);

int main() {
  printf("Before calling worker threads to work on NUM its value is: %d\n",
         num);

  pthread_cond_init(&ready, NULL);
  pthread_t thread1, thread2;
  // initializing the mutex by ack the lock
  if (pthread_mutex_init(&lock, NULL) != 0) {
    perror("Mutex Initialization Failed\n");
    return 1;
  }

  printf("Ready to Increase %d\n", num);

  // creating the threads
  if (pthread_create(&thread1, NULL, count_up, "Thread A") != 0) {
    perror("Thread 1 could not be created\n");
    return 1;
  }

  if (pthread_create(&thread2, NULL, count_up, "Thread B") != 0) {
    perror("Thread 2 could not be created\n");
    return 1;
  }

  // join the threads
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // destroy the mutex
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&ready);

  printf("The Final Value of NUM after both worker threads incrementing is: "
         "%d\n",
         num);
}
