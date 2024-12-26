#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void unix_error_num(char *msg, int err) {
  fprintf(stderr, "%s: %s\n", msg, strerror(err));
  exit(0);
}

void unix_error(char *msg) { unix_error_num(msg, errno); }

void *Malloc(size_t size) {
  void *m = malloc(size);
  if (m == NULL)
    unix_error("Malloc error");
  return m;
}

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                    void *(*fn)(void *), void *arg) {
  int ret;
  if ((ret = pthread_create(tid, attr, fn, arg)) != 0)
    unix_error_num("Create thread error", ret);
}

void Pthread_join(pthread_t tid, void **valptr) {
  int ret;
  if ((ret = pthread_join(tid, valptr)) != 0)
    unix_error_num("Join thread error", ret);
}

void Pthread_mutex_lock(pthread_mutex_t *lock) {
  int ret;
  if ((ret = pthread_mutex_lock(lock)) != 0)
    unix_error_num("Mutex lock error", ret);
}

void Pthread_mutex_unlock(pthread_mutex_t *lock) {
  int ret;
  if ((ret = pthread_mutex_unlock(lock)) != 0)
    unix_error_num("Mutex unlock error", ret);
}

void Pthread_mutex_init(pthread_mutex_t *lock) {
  int ret;
  if ((ret = pthread_mutex_init(lock, NULL)) != 0)
    unix_error_num("Mutex init error", ret);
}

void Pthread_mutex_destroy(pthread_mutex_t *lock) {
  int ret;
  if ((ret = pthread_mutex_destroy(lock)) != 0)
    unix_error_num("Mutex destroy error", ret);
}

void Pthread_cond_init(pthread_cond_t *cond) {
  int ret;
  if ((ret = pthread_cond_init(cond, NULL)) != 0)
    unix_error_num("Condvar init error", ret);
}

void Pthread_cond_destroy(pthread_cond_t *cond) {
  int ret;
  if ((ret = pthread_cond_destroy(cond)) != 0)
    unix_error_num("Condvar destroy error", ret);
}

void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *lock) {
  int ret;
  if ((ret = pthread_cond_wait(cond, lock)) != 0)
    unix_error_num("Condvar wait error", ret);
}

void Pthread_cond_signal(pthread_cond_t *cond) {
  int ret;
  if ((ret = pthread_cond_signal(cond)) != 0)
    unix_error_num("Condvar signal error", ret);
}

void Pthread_cond_broadcast(pthread_cond_t *cond) {
  int ret;
  if ((ret = pthread_cond_broadcast(cond)) != 0)
    unix_error_num("Condvar broadcast error", ret);
}
