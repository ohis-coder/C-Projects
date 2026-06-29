#include <stdio.h>

    int dumb_increase(int old_value) {
	int reformed_value = 0;

	// using ARM64 assembly to increment old value by 1 and return
	// in reformed value!!! 

	// please note the function name, dumb increase...
	// i am playing w assembly that is all 

	__asm__( 
		"mov w2, w1%\n\t"
		"mov w3, w1\n\t"
		"add w3, w3, #1\n\t"
		"mov %w0, w3\n\t"
		: "=r" (reformed_value)
		: "r" (old_value)
		: "x2", "x3"
	);

	return reformed_value;
    }

int main() {
dumb_increase(5);
}
