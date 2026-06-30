#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

extern int data;

int dumb_increase_one(data);

#endif
