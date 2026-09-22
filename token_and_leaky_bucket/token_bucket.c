#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_TOKENS 10.0 // Maximum capacity of the bucket
#define REFILL_RATE 2.0 // Tokens earned per second

typedef struct {
  double tokens;
  double last_update_time;
  pthread_mutex_t lock; // Protects bucket state from concurrent threads
} TokenBucket;

// Initialize your bucket safely
void token_bucket_init(TokenBucket *bucket, double start_time) {
  bucket->tokens = MAX_TOKENS;
  bucket->last_update_time = start_time;
  pthread_mutex_init(&bucket->lock, NULL);
}

// Clean up the mutex when done
void token_bucket_destroy(TokenBucket *bucket) {
  pthread_mutex_destroy(&bucket->lock);
}

bool token_bucket_check(TokenBucket *bucket, double now) {
  //  LOCK MUTEX: Secure the shared memory before reading/writing
  pthread_mutex_lock(&bucket->lock);

  // 1. Calculate time elapsed using local stack variables
  double time_elapsed = now - bucket->last_update_time;
  double tokens_earned = time_elapsed * REFILL_RATE;

  // 2. Perform the math and normalize LOCALLY first (Senior Engineer Fix)
  double calculated_tokens = bucket->tokens + tokens_earned;
  if (calculated_tokens > MAX_TOKENS) {
    calculated_tokens = MAX_TOKENS;
  }

  // 3. Commit clean, normalized values to the shared structure
  bucket->tokens = calculated_tokens;
  bucket->last_update_time = now;

  // 4. Evaluate if the request can spend a token to pass
  if (bucket->tokens >= 1.0) {
    bucket->tokens -= 1.0; // Spend the token

    pthread_mutex_unlock(&bucket->lock); // ALWAYS unlock before returning!
    return true;                         // Request Allowed
  }

  pthread_mutex_unlock(&bucket->lock); // ALWAYS unlock before returning!
  return false;                        // Rate-limited
}
