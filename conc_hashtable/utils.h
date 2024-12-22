#include <stddef.h>
#include <stdio.h>
#include <pthread.h>

#define TEST printf("TEST %s: %s \n", __FILE__, __func__)

void *Malloc(size_t size);
void Pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                    void *(*fn)(void *), void *arg);
void Pthread_join(pthread_t tid, void **valptr);
void Pthread_mutex_lock(pthread_mutex_t *lock);
void Pthread_mutex_unlock(pthread_mutex_t *lock);
void Pthread_mutex_init(pthread_mutex_t *lock, pthread_mutexattr_t *attr);
void Pthread_mutex_destroy(pthread_mutex_t *lock);
