/*
 * Implementation of HashTable (set) with one lock per bucket.
 */
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#include "mht.h"
#include "utils.h"

#define MAX_BUCKETS 1000

size_t hash_key(int key, size_t buckets) {
  if (key == INT_MIN)
    return 0;

  /* size_t has a higher rank than int so the result of % is size_t */
  // return abs(key) % buckets;
  return abs(key) % buckets;
}

HashTable *hash_table_new(size_t buckets) {
  if (buckets > MAX_BUCKETS)
    return NULL;

  HashTable *ht =
      (HashTable *)Malloc(sizeof(HashTable) + sizeof(LinkedList) * buckets);
  if (ht == NULL)
    return NULL;

  ht->buckets = buckets;
  for (size_t i = 0; i < buckets; i++) {
    cll_init(&ht->lists[i]);
  }

  return ht;
}

void hash_table_destroy(HashTable *ht) {
  for (size_t i = 0; i < ht->buckets; i++) {
    cll_clear(&ht->lists[i]);
  }
  free(ht);
}

bool hash_table_insert(HashTable *ht, int key) {
  size_t bkt = hash_key(key, ht->buckets);
  return cll_add_not_exists(&ht->lists[bkt], key);
}

bool hash_table_exists(HashTable *ht, int key) {
  size_t bkt = hash_key(key, ht->buckets);
  bool exists = cll_exists(&ht->lists[bkt], key);
  return exists;
}

bool hash_table_remove(HashTable *ht, int key) {
  size_t bkt = hash_key(key, ht->buckets);
  bool removed = cll_remove(&ht->lists[bkt], key);
  return removed;
}
