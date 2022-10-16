#include <iostream>
#include <pthread.h>

pthread_barrier_t barrier;
pthread_barrierattr_t attr;
unsigned count;
int ret = pthread_barrier_init(&barrier, &attr, count);

pthread_barrier_t pbarrier;
int ret = pthread_barrier_wait(&barrier);
