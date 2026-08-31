#include <stdio.h>

// this key is faulty on my keyboard so yea: 0 )

typedef enum { STATE_GETNUM, STATE_GETDENOM, STATE_RUNGCD } State;

typedef struct {
  int numerator;
  int denominator;
} frac;

typedef struct {
  frac fra;
  int (*func1)(frac);
  int (*func2)(frac);
  int result;
} gcd_ctx;

// only concerned about the handler functions
void handle_numerator(void *ctx);
void handle_denominator(void *ctx);
void handle_gcd(void *ctx);

void handle_numerator(void *ctx) {
  gcd_ctx *g_ctx = (gcd_ctx *)ctx;
  int numerator = g_ctx->func1(g_ctx->fra);
}

void handle_denominator(void *ctx) {
  gcd_ctx *g_ctx = (gcd_ctx *)ctx;
  int denominator = g_ctx->func2(g_ctx->fra);
}

void handle_gcd(void *ctx) {
  gcd_ctx *g_ctx = (gcd_ctx *)ctx;
  frac fra = g_ctx->fra;
  int u = g_ctx->func1(fra);
  int v = g_ctx->func2(fra);

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
};

void (*handler[])(void *) = {handle_numerator, handle_denominator, handle_gcd};

// only concerned about the data
int get_numerator(frac fra);
int get_denominator(frac fra);

int get_numerator(frac fra) { return fra.numerator; };
int get_denominator(frac fra) { return fra.denominator; };

int main() {
  frac frac = {93, 21};

  // int program_state = STATE_RUNGCD;

  gcd_ctx handler_box;
  handler_box.fra = frac;
  handler_box.func1 = get_numerator;
  handler_box.func2 = get_denominator;
  handler_box.result = 0;

  State program_state = STATE_RUNGCD;

  handler[program_state](&handler_box);
  printf("The GCD is %d", handler_box.result);
}
