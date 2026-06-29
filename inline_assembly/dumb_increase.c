#include <stdio.h>

int dumb_increase(int old_value) {
  int reformed_value = 0;

  __asm__("addl $1, %1\n\t" // 1. Add the constant 1 directly to the input
                            // register (%1)
          "movl %1, %0\n\t" // 2. Handover: Copy that updated value into the
                            // output register (%0)
          : "=r"(reformed_value) // %0 maps cleanly to reformed_value
          : "r"(old_value)       // %1 maps cleanly to old_value
  );

  return reformed_value;
}

int main() {
  int result = dumb_increase(5);
  printf("Result of dumb_increase: %d\n", result); // Will print 6!
  return 0;
}
