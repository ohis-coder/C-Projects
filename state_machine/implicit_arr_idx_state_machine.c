#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

// 1. The Enum dictates the exact numerical indices (0, 1, 2, 3)
typedef enum {
  STATE_IDLE,
  STATE_PROCESS_BURST,
  STATE_SHAPE_TRAFFIC,
  STATE_SHUTDOWN
} SystemState;

// Global tracking variables
SystemState current_state = STATE_IDLE;
bool system_running = true;

// Concurrency tools to protect and signal the outside state
pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t state_signal = PTHREAD_COND_INITIALIZER;

// Worker Functions
void do_idle() { printf("[WATCHER] System is idle. Waiting for traffic...\n"); }
void do_process_burst() {
  printf("[WATCHER] Blasting through sudden token burst traffic!\n");
}
void do_shape_traffic() {
  printf("[WATCHER] Leaky bucket logic engaged. Smoothing stream.\n");
}

// 2. YOUR TIGHT JUMP TABLE: Implicitly maps 0, 1, 2, 3 based on array order
void (*state_table[])() = {
    do_idle,          // Index 0 -> STATE_IDLE
    do_process_burst, // Index 1 -> STATE_PROCESS_BURST
    do_shape_traffic, // Index 2 -> STATE_SHAPE_TRAFFIC
    NULL              // Index 3 -> STATE_SHUTDOWN (Handled explicitly)
};

// THE ASYNCHRONOUS WATCHER THREAD
void *watcher_thread_func(void *arg) {
  pthread_mutex_lock(&state_mutex);

  while (system_running) {
    // Sleep until an outside event updates the enum and signals us
    pthread_cond_wait(&state_signal, &state_mutex);

    if (current_state == STATE_SHUTDOWN) {
      system_running = false;
      break;
    }

    // Your exact execution line! Ultra-fast, direct array index jump
    if (state_table[current_state] != NULL) {
      state_table[current_state]();
    }
  }

  pthread_mutex_unlock(&state_mutex);
  printf("[WATCHER] Watcher thread safely terminating.\n");
  return NULL;
}

// Global thread-safe state updater
void update_system_state(SystemState new_state) {
  pthread_mutex_lock(&state_mutex);
  current_state = new_state;
  pthread_cond_signal(&state_signal); // Signal the watcher to wake up and jump
  pthread_mutex_unlock(&state_mutex);
}

int main() {
  pthread_t watcher;
  pthread_create(&watcher, NULL, watcher_thread_func, NULL);

  printf("[MAIN] Main system active. Simulating event bursts...\n");
  sleep(1);

  // Main thread updates the state outside; background thread executes it
  // asynchronously
  printf("[MAIN] Sudden spike detected!\n");
  update_system_state(STATE_PROCESS_BURST);
  sleep(2);

  printf("[MAIN] Traffic sustained. Shifting gears...\n");
  update_system_state(STATE_SHAPE_TRAFFIC);
  sleep(2);

  printf("[MAIN] Commencing shutdown...\n");
  update_system_state(STATE_SHUTDOWN);

  pthread_join(watcher, NULL);
  printf("[MAIN] Execution completely finished.\n");
  return 0;
}
