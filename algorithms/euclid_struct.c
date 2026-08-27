#include <stdio.h>

// this key is faulty on my keyboard so yea: 0 )

typedef struct {
  int numerator;
  int denominator;
} frac;

int get_numerator(frac);
int get_denominator(frac);
int gcd(frac fra, int (*func1)(frac), int (*func2)(frac));

int get_numerator(frac frac) {
  int numerator = frac.numerator;
  return numerator;
}

int get_denominator(frac frac) {
  int denominator = frac.denominator;
  return denominator;
}

int gcd(frac fra, int (*func1)(frac), int (*func2)(frac)) {
  int u = func1(fra);
  int v = func2(fra);

  int t;

  while (u > 0) {
    if (u < v) {
      t = u;
      u = v;
      v = t;
    }
    u = u - v;
  }
  return v;
}

int main() {
  frac frac = {93, 21};

  int result = gcd(frac, get_numerator, get_denominator);
  printf("The GCD is %d", result);
}
