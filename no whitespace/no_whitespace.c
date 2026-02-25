#include <stdio.h>

int main() {
	int c;
	int last_c;
	
	last_c = EOF;
	
	/*as long as we are not at the end of file*/
	/*keep the check for blank space alive*/
	
	while ((c=getchar()) != EOF) {
		if (c != ' ') {
			putchar(c);
		}
		else {
			if (last_c != ' ')
				putchar(c)
		}
		last_c = c
	}
	
	return 0;
}