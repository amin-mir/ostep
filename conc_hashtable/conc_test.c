#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "hasht.h"
#include "utils.h"

#define LOOPS 100000

typedef struct {
  int id;
  HashTable *ht;
} TArgs;

void *write_read_hash_table(void *arg) {
  TArgs *args = (TArgs *)arg;
  int key = args->id * LOOPS;

  int i;
  for (i = 0; i < LOOPS; i++) {
    hash_table_insert(args->ht, key + i);
  }

  for (i = 0; i < LOOPS; i++) {
    assert(hash_table_exists(args->ht, key + i));
  }
  assert(!hash_table_exists(args->ht, -1000));
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Provide required arguments: num_threads <num_buckets>\n");
    return 1;
  }

  size_t num_buckets = 100;
  if (argc >= 3) {
    num_buckets = atoi(argv[2]);
  }

  int num_threads = atoi(argv[1]);
  pthread_t tids[num_threads];
  /* Create the thread arguments on the main stack and pass a pointer to these
   * to each thread. */
  TArgs targs[num_threads];

  HashTable *ht = hash_table_new(num_buckets);

  clock_t start = clock();

  int i = 0;
  for (i = 0; i < num_threads; i++) {
    targs[i].id = i;
    targs[i].ht = ht;
    Pthread_create(&tids[i], NULL, write_read_hash_table, (void *)&targs[i]);
  }

  for (i = 0; i < num_threads; i++) {
    Pthread_join(tids[i], NULL);
  }

  clock_t end = clock();
  double time = (double)(end - start) / CLOCKS_PER_SEC;
  printf("It took = %f\n", time);
}
