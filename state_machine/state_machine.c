#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h> // For sleep simulation

typedef enum {
  STATE_IDLE,
  STATE_PROCESS_BURST,
  STATE_SHAPE_TRAFFIC,
  STATE_SHUTDOWN,
  NUM_STATES
} SystemState;

// Global control variables
SystemState current_state = STATE_IDLE;
bool system_running = true;

// The Watcher's Synchronization Tools
pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t state_signal = PTHREAD_COND_INITIALIZER;

// State Logic Functions
void do_idle() { printf("[WATCHER] System is idle. Waiting for traffic...\n"); }
void do_process_burst() {
  printf("[WATCHER] Blasting through sudden token burst traffic!\n");
}
void do_shape_traffic() {
  printf("[WATCHER] Leaky bucket logic engaged. Smoothing stream.\n");
}

// The Jump Table Array matching the enum
void (*state_table[NUM_STATES])() = {
    [STATE_IDLE] = do_idle,
    [STATE_PROCESS_BURST] = do_process_burst,
    [STATE_SHAPE_TRAFFIC] = do_shape_traffic,
    [STATE_SHUTDOWN] = NULL // Handled explicitly
};

// THE GENIUS WATCHER THREAD
void *watcher_thread_func(void *arg) {
  pthread_mutex_lock(&state_mutex);

  while (system_running) {
    // Sleep until someone changes the state and sends a signal!
    // This drops the mutex so others can change the state while we sleep.
    pthread_cond_wait(&state_signal, &state_mutex);

    // --- WOKE UP! Someone changed the state ---
    if (current_state == STATE_SHUTDOWN) {
      system_running = false;
      break;
    }

    // Execute the logic tied to the outside state instantly using our jump
    // table
    if (state_table[current_state] != NULL) {
      state_table[current_state]();
    }
  }

  pthread_mutex_unlock(&state_mutex);
  printf("[WATCHER] Watcher thread safely terminating.\n");
  return NULL;
}

// Function to cleanly change the state from anywhere in the app
void update_system_state(SystemState new_state) {
  pthread_mutex_lock(&state_mutex);
  current_state = new_state;          // Change the state outside
  pthread_cond_signal(&state_signal); // SEND THE SIGNAL TO THE WATCHER!
  pthread_mutex_unlock(&state_mutex);
}

int main() {
  pthread_t watcher;
  // Spin up your background watcher thread
  pthread_create(&watcher, NULL, watcher_thread_func, NULL);

  printf("[MAIN] Main system active. Simulating events...\n");
  sleep(1);

  // Simulate real-world network traffic changing states outside
  printf("[MAIN] Sudden spike detected!\n");
  update_system_state(STATE_PROCESS_BURST);
  sleep(2);

  printf("[MAIN] Traffic sustained. Shifting gears...\n");
  update_system_state(STATE_SHAPE_TRAFFIC);
  sleep(2);

  // Shut down the system
  printf("[MAIN] Commencing shutdown...\n");
  update_system_state(STATE_SHUTDOWN);

  // Wait for the watcher thread to finish cleaning up
  pthread_join(watcher, NULL);
  printf("[MAIN] Execution completely finished.\n");
  return 0;
}
