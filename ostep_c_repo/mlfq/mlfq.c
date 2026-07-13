#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define Q2_AYS 2
#define Q1_AYS 4
#define Q0_AYS 8
#define MAX_TASKS 100

// Task structure Definition
typedef struct {
  char name[20];
  int yt;  // yield time
  int dur; // duration
} Task;

// Queue Structure Definition
typedef struct {
  Task tasks[MAX_TASKS];
  int front;
  int rear;
  int count;
} Queue;

// Global simulation variables
volatile int gt = 5; // Global Time (5 seconds)
pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;

// Define queues
Queue q2, q1, q0;

// Queue Helper Functions
void init_queue(Queue *q) {
  q->front = 0;
  q->rear = -1;
  q->count = 0;
}

bool is_empty(Queue *q) { return q->count == 0; }

void push(Queue *q, Task t) {
  if (q->count < MAX_TASKS) {
    q->rear = (q->rear + 1) % MAX_TASKS;
    q->tasks[q->rear] = t;
    q->count++;
  }
}

Task pop(Queue *q) {
  Task t = {"", 0, 0};
  if (!is_empty(q)) {
    t = q->tasks[q->front];
    q->front = (q->front + 1) % MAX_TASKS;
    q->count--;
  }
  return t;
}

// Background thread function to decrement global time (gt)
void *global_timer_func(void *arg) {
  while (1) {
    sleep(1); // Wait 1 second
    pthread_mutex_lock(&timer_mutex);
    if (gt > 0) {
      gt--;
      printf("\n [Timer Signal] Global Time left: %ds\n", gt);
      if (gt == 0) {
        printf(" [Timer Signal] gt reached 0! Stopping simulation loop.\n\n");
        pthread_mutex_unlock(&timer_mutex);
        break;
      }
    } else {
      pthread_mutex_unlock(&timer_mutex);
      break;
    }
    pthread_mutex_unlock(&timer_mutex);
  }
  return NULL;
}

// Main MLFQ Scheduler Engine
int main() {
  // 1. Initialize our 3 queues
  init_queue(&q2);
  init_queue(&q1);
  init_queue(&q0);

  // 2. Load some sample tasks into high priority Q2
  push(&q2, (Task){"TaskA", 3, 10}); // Case: yt >= Q2_AYS (drops priority)
  push(&q2,
       (Task){"TaskB", 1, 1}); // Case: yt < Q2_AYS, short duration (finishes)
  push(&q2, (Task){"TaskC", 1,
                   5}); // Case: yt < Q2_AYS, long duration (drops priority)

  printf(" Starting MLFQ Simulation (gt = %ds)...\n", gt);

  // 3. Start the background global time thread
  pthread_t timer_thread;
  pthread_create(&timer_thread, NULL, global_timer_func, NULL);

  // 4. MLFQ Execution Loops
  while (1) {
    // Check global timer expiration safely
    pthread_mutex_lock(&timer_mutex);
    if (gt <= 0) {
      pthread_mutex_unlock(&timer_mutex);
      break;
    }
    pthread_mutex_unlock(&timer_mutex);

    // Process Queue 2 (High Priority)
    if (!is_empty(&q2)) {
      Task t = pop(&q2);
      printf("Processing [%s] in Q2 (dur: %d, yt: %d) -> ", t.name, t.dur,
             t.yt);

      if (t.yt < Q2_AYS) {
        t.dur = t.dur - Q2_AYS;
        if (t.dur > 0) {
          printf("Yield valid, duration remaining. Dropping to Q1.\n");
          push(&q1, t);
        } else {
          printf("Task Completed successfully in Q2.\n");
        }
      } else {
        printf(
            "Yield invalid (yt >= ays). Dropping priority directly to Q1.\n");
        push(&q1, t);
      }
      sleep(1); // Simulate work execution time
      continue;
    }

    // Process Queue 1 (Medium Priority)
    if (!is_empty(&q1)) {
      Task t = pop(&q1);
      printf("Processing [%s] in Q1 (dur: %d, yt: %d) -> ", t.name, t.dur,
             t.yt);

      if (t.yt < Q1_AYS) {
        t.dur = t.dur - Q1_AYS;
        if (t.dur > 0) {
          printf("Yield valid, duration remaining. Dropping to Q0.\n");
          push(&q0, t);
        } else {
          printf("Task Completed successfully in Q1.\n");
        }
      } else {
        printf(
            "Yield invalid (yt >= ays). Dropping priority directly to Q0.\n");
        push(&q0, t);
      }
      sleep(1); // Simulate work execution time
      continue;
    }

    // Process Queue 0 (Low Priority)
    if (!is_empty(&q0)) {
      Task t = pop(&q0);
      printf("Processing [%s] in Q0 (dur: %d, yt: %d) -> ", t.name, t.dur,
             t.yt);

      if (t.yt < Q0_AYS) {
        t.dur = t.dur - Q0_AYS;
        if (t.dur > 0) {
          printf("Duration remaining. Re-queuing to Q0 tail.\n");
          push(&q0, t);
        } else {
          printf("Task Completed successfully in Q0.\n");
        }
      } else {
        printf("Yield invalid in Q0. Terminating/Dropping task execution.\n");
      }
      sleep(1); // Simulate work execution time
      continue;
    }

    // If all queues are empty, stop scheduler loop
    if (is_empty(&q2) && is_empty(&q1) && is_empty(&q0)) {
      printf("\nAll queues are empty. Scheduler completed execution.\n");
      break;
    }
  }

  // Wait for the background timer thread to finish clean up
  pthread_join(timer_thread, NULL);
  printf(" Simulation finished.\n");
  return 0;
}
