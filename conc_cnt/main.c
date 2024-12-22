#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_INCREMENTS 1000000
static int threshold = 100;

typedef struct {
  unsigned c;
  pthread_mutex_t lock;
} counter;

void Pthread_mutex_lock(pthread_mutex_t *lock) {
  int locked = pthread_mutex_lock(lock);
  if (locked != 0) {
    printf("Acquire lock error: %s\n", strerror(locked));
    exit(1);
  }
}

void Pthread_mutex_unlock(pthread_mutex_t *lock) {
  assert(pthread_mutex_unlock(lock) == 0);
}

// void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void
// *(*fn)(void *), void *arg) {
//   int ret = pthread_create(tid, attr, fn, arg);
//   if (ret != 0) {
//     fprintf(stderr, "Create thread error: %s\n", strerror(ret));
//     exit(1);
//   }
// }

#define Pthread_mutex_init(lock, attr)                                         \
  assert(pthread_mutex_init((lock), (attr)) == 0)
#define Pthread_mutex_destroy(lock) assert(pthread_mutex_destroy((lock)) == 0)
#define Pthread_create(tid, attr, fn, arg)                                     \
  assert(pthread_create((tid), (attr), (fn), (arg)) == 0);
#define Pthread_join(tid, valptr) assert(pthread_join((tid), (valptr)) == 0)

void counter_init(counter *cnt) {
  cnt->c = 0;
  Pthread_mutex_init(&cnt->lock, NULL);
}

void counter_free_lock(counter *cnt) { Pthread_mutex_destroy(&cnt->lock); }

void counter_inc(counter *cnt, int amnt) {
  Pthread_mutex_lock(&cnt->lock);
  cnt->c += amnt;
  Pthread_mutex_unlock(&cnt->lock);
}

int counter_get(counter *cnt) {
  int res;
  Pthread_mutex_lock(&cnt->lock);
  res = cnt->c;
  Pthread_mutex_unlock(&cnt->lock);
  return res;
}

void *increment_counter(void *arg) {
  counter *cnt = (counter *)arg;
  int lcount = 0;

  int i;
  for (i = 0; i < NUM_INCREMENTS; i++) {
    if (++lcount == threshold) {
      counter_inc(cnt, threshold);
      lcount = 0;
    }
  }

  /* Add the remaineder before returning */
  counter_inc(cnt, lcount);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Provide num_threads and threshold args\n");
    return 1;
  }

  int num_threads = atoi(argv[1]);
  threshold = atoi(argv[2]);

  counter cnt;
  counter_init(&cnt);

  pthread_t tids[num_threads];
  // pthread_t *tids = malloc(num_threads * sizeof(pthread_t));
  // if (tids == NULL) {
  //   fprintf(stderr, "Malloc error: %s\n", strerror(errno));
  // }

  clock_t start = clock();

  int i;
  for (i = 0; i < num_threads; i++) {
    Pthread_create(&tids[i], NULL, (void *(*)(void *))increment_counter,
                   (void *)&cnt);
  }

  for (i = 0; i < num_threads; i++) {
    Pthread_join(tids[i], NULL);
  }

  clock_t end = clock();
  double time = (double)(end - start) / CLOCKS_PER_SEC;
  printf("count = %d, time spent = %f\n", counter_get(&cnt), time);
  assert(counter_get(&cnt) == NUM_INCREMENTS * num_threads);

  /* Free both the counter and thread ID array */
  counter_free_lock(&cnt);
  // free(tids);
}
