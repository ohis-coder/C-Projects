#include <stdio.h>

int main() {
	int c, nl;
	
	nl = 0; 
	
	while ((c = getchar())!= EOF)
		if (c == '\n')
			nl++;
	printf("There are %d number of lines\n", nl);
		
	return 0;
}