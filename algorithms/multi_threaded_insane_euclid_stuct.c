#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

// this key is faulty on my keyboard so yea: 0 )

typedef enum { STATE_RUNGCD } State;

typedef struct {
  int numerator;
  int denominator;
} frac;

typedef struct {
  frac fra;
  void *(*func1)(void *);
  void *(*func2)(void *);
  int result;

  pthread_mutex_t lock;
} gcd_ctx;

// only concerned about the data
void *get_numerator(void *);
void *get_denominator(void *);

void *get_numerator(void *arg) {
  frac *fra = (frac *)arg;

  return (void *)(uintptr_t)fra->numerator;
}

void *get_denominator(void *arg) {
  frac *fra = (frac *)arg;

  return (void *)(uintptr_t)fra->denominator;
}

// only concerned about the handler functions
void *handle_gcd(void *ctx);

void *handle_gcd(void *ctx) {
  gcd_ctx *g_ctx = (gcd_ctx *)ctx;

  pthread_mutex_lock(&g_ctx->lock);

  int u = (int)(uintptr_t)g_ctx->func1(&(g_ctx->fra));
  int v = (int)(uintptr_t)g_ctx->func2(&(g_ctx->fra));

  int t;

  while (u > 0) {
    if (u < v) {
      t = u;
      u = v;
      v = t;
    }
    u = u - v;
  }
  g_ctx->result = v;

  pthread_mutex_unlock(&g_ctx->lock);
  return NULL;
}

void *(*handler[])(void *) = {handle_gcd};

int main() {
  frac frac = {93, 21};

  gcd_ctx handler_box;
  handler_box.fra = frac;
  handler_box.func1 = get_numerator;
  handler_box.func2 = get_denominator;
  handler_box.result = 0;

  State program_state = STATE_RUNGCD;

  handler[program_state](&handler_box);
  printf("The GCD is %d", handler_box.result);
}
