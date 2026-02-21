#include <stdio.h>

int main() {
	float fahr, celsius;
	float lower, upper, step;
	
	lower = 0;
	upper = 300;
	step = 20;
	
	celsius = upper;
	printf("c\t f\n");
	while (celsius >= lower) {
		fahr = (celsius * (9.0 / 5.0)) + 32.0;
		printf("%.0f \t %.1f\n", celsius, fahr);
		celsius = celsius - step;
	}
	
	
	return 0;
}