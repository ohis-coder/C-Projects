#ifndef HEADER_H
#define HEADER_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

extern atomic_int data;
// pthread_mutex_t lock;

void *dumb_increase_one(void *arg);
int run();

#endif
