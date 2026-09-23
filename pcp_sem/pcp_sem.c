#include <pthread.h>
#include <semaphore.h> // Brings in sem_t
#include <stdbool.h>
#include <stdio.h>

#define QUEUE_CAPACITY 5

typedef struct {
  int data[QUEUE_CAPACITY];
  int head;
  int tail;

  pthread_mutex_t lock;  // Protects the queue arrays/pointers
  sem_t space_available; // Counts empty slots (Initialized to QUEUE_CAPACITY)
  sem_t data_available;  // Counts filled slots  (Initialized to 0)
} SynchronizedQueue;

// Initialize the queue state
void queue_init(SynchronizedQueue *q) {
  q->head = 0;
  q->tail = 0;
  pthread_mutex_init(&q->lock, NULL);

  // sem_init(semaphore_pointer, pshared_flag, initial_value)
  sem_init(&q->space_available, 0, QUEUE_CAPACITY); // Starts with 5 empty spots
  sem_init(&q->data_available, 0, 0);               // Starts with 0 items
}

// Clean up OS resources
void queue_destroy(SynchronizedQueue *q) {
  pthread_mutex_destroy(&q->lock);
  sem_destroy(&q->space_available);
  sem_destroy(&q->data_available);
}

// THE PRODUCER (Supply - User traffic dropping packets into the buffer)
void queue_produce(SynchronizedQueue *q, int packet) {
  // 1. Decrement space semaphore. If count is 0 (full), this thread instantly
  // sleeps!
  sem_wait(&q->space_available);

  // 2. Lock the mutex to safely touch the array pointers
  pthread_mutex_lock(&q->lock);

  // Critical Section: Add item to the circular buffer
  q->data[q->tail] = packet;
  q->tail = (q->tail + 1) % QUEUE_CAPACITY;

  pthread_mutex_unlock(&q->lock);

  // 3. Increment data semaphore. Automatically wakes up a sleeping consumer!
  sem_post(&q->data_available);
}

// THE CONSUMER (Demand - Server/Worker thread executing heavy tasks)
int queue_consume(SynchronizedQueue *q) {
  // 1. Decrement data semaphore. If count is 0 (empty), this thread instantly
  // sleeps!
  sem_wait(&q->data_available);

  // 2. Lock the mutex to safely touch the array pointers
  pthread_mutex_lock(&q->lock);

  // Critical Section: Remove item from the circular buffer
  int packet = q->data[q->head];
  q->head = (q->head + 1) % QUEUE_CAPACITY;

  pthread_mutex_unlock(&q->lock);

  // 3. Increment space semaphore. Automatically wakes up a sleeping producer!
  sem_post(&q->space_available);

  return packet;
}
