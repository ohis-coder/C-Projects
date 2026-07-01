#include <stdio.h>
#include <ctype.h>

// this key is faulty on my keyboard so yea: 0 )


int getnum(){
	int i, c;
	printf("Input Task Completion Time (< 9 secs bro)\n");
	for (int i = 0; i < 1 && (c = getchar()) != EOF; i++) {
		if (!isdigit(c) || c >= '9'){
			printf("input a valid number dawg\n");
			break;
		}
		i++;
	}
	return c - '0';
}

int main(){int num = getnum(); printf("%d\n", num); return 0;}
