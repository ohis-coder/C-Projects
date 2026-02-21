#include <stdio.h>

	/* print Fahrenheit-Celsius table
		for fahr = 0, 20, ..., 300 */ 

int main() {
	float celsius, fahr;
	float lower, upper, step;
	
	lower = 0;
	upper = 300;
	step = 20;
	
	fahr = lower;
	printf("f\t c\n");
	while (fahr <= upper) {
		celsius = (5.0/ 9.0) * (fahr - 32.0);
		printf("%.0f \t %.1f\n", fahr, celsius);
		fahr = fahr + step;
	}
	
	
	return 0;
}