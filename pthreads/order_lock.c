#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  int page_id;
  pthread_mutex_t mutex;
} DBPage;

// High-performance deadlock-free locking function for ANY number of pages
void lock_multiple_pages(DBPage *pages[], int total_pages) {
  if (total_pages <= 0)
    return;
  if (total_pages == 1) {
    pthread_mutex_lock(&pages[0]->mutex);
    return;
  }

  // 1. Sort the pages array based on their raw 8-byte RAM addresses
  // we use a simple, low-overhead Insertion Sort (perfect for small arrays)
  for (int i = 1; i < total_pages; i++) {
    DBPage *key = pages[i];
    int j = i - 1;

    // Smuggle the pointers into uintptr_t numbers to compare physical RAM
    // placement
    while (j >= 0 && (uintptr_t)&pages[j]->mutex > (uintptr_t)&key->mutex) {
      pages[j + 1] = pages[j];
      j = j - 1;
    }
    pages[j + 1] = key;
  }

  // 2. Lock them in strict ascending order based on their physical address
  // footprint Because the order is sorted, NO circular wait can ever form!
  for (int i = 0; i < total_pages; i++) {
    pthread_mutex_lock(&pages[i]->mutex);
  }
}

// Mirroring function to safely release the locks
void unlock_multiple_pages(DBPage *pages[], int total_pages) {
  // Order doesn't matter for unlocking, but scanning the sorted sequence is
  // clean
  for (int i = 0; i < total_pages; i++) {
    pthread_mutex_unlock(&pages[i]->mutex);
  }
}
