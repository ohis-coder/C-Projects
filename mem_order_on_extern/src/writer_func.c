#include "../include/header.h"

// this key is faulty on my keyboard so yea: 0 )

void dumb_increase_one(int data) {
	for (i = 0; i <= 5; i++) {
		data++;
	}
}

/*
int dumb_increase_two(int data) {
	for (i = 0; i <= 5; i++) {
		data++;
		return data;
	}
}
*/

void *run(void) {
pthread_t t1, t2;

if (pthread_create(&t1, NULL, dumb_increase_one, data) != 0){
	perror("Failed to create t1");
}

if (pthread_create(&t2, NULL, dumb_increase_one, data) != 0){
	perror("Failed to create t2");
}

pthread_join(t1, NULL);
pthread_join(t2, NULL);

return data;
}
