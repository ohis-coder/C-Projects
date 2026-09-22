#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define BUCKET_CAPACITY 5.0 // Max queue size before dropping requests
#define LEAK_RATE 1.0       // Requests processed per second

typedef struct {
  double water_level;      // Current simulated queue depth
  double last_update_time; // Last time a request hit this bucket
  pthread_mutex_t lock;    // Protects bucket state from concurrent threads
} LeakyBucket;

// Initialize your bucket safely
void leaky_bucket_init(LeakyBucket *bucket, double start_time) {
  bucket->water_level = 0.0;
  bucket->last_update_time = start_time;
  pthread_mutex_init(&bucket->lock, NULL);
}

// Clean up the mutex when done
void leaky_bucket_destroy(LeakyBucket *bucket) {
  pthread_mutex_destroy(&bucket->lock);
}

bool leaky_bucket_check(LeakyBucket *bucket, double now) {
  // LOCK MUTEX: Secure the shared memory before reading/writing
  pthread_mutex_lock(&bucket->lock);

  // 1. Calculate elapsed time using local stack variables
  double time_elapsed = now - bucket->last_update_time;
  double water_leaked = time_elapsed * LEAK_RATE;

  // 2. Perform the math and normalize LOCALLY first (Senior Engineer Fix)
  double calculated_water = bucket->water_level - water_leaked;
  if (calculated_water < 0.0) {
    calculated_water = 0.0;
  }

  // 3. Commit clean, normalized values to the shared structure
  bucket->water_level = calculated_water;
  bucket->last_update_time = now;

  // 4. Check if adding this request overflows our capacity
  if (bucket->water_level + 1.0 <= BUCKET_CAPACITY) {
    bucket->water_level += 1.0; // Add the request to the virtual queue

    pthread_mutex_unlock(&bucket->lock); // ALWAYS unlock before returning!
    return true;                         // Request Allowed
  }

  pthread_mutex_unlock(&bucket->lock); // ALWAYS unlock before returning!
  return false;                        // Rate-limited (Buffer Overflow)
}
