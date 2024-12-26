#include <assert.h>

#include "../utils.h"
#include "mht.h"

#define BUCKETS 100

static void test_operations_empty_hash_table() {
  TEST;
  HashTable *ht = hash_table_new(BUCKETS);

  assert(!hash_table_exists(ht, 10));
  assert(!hash_table_exists(ht, 19));
  hash_table_destroy(ht);
}

static void test_insert() {
  TEST;
  HashTable *ht = hash_table_new(BUCKETS);

  /* HashTable won't store redundant keys */
  hash_table_insert(ht, 1);
  hash_table_insert(ht, 1);
  hash_table_insert(ht, 1);
  assert(hash_table_exists(ht, 1));
  assert(hash_table_remove(ht, 1));
  assert(!hash_table_remove(ht, 1));
  assert(!hash_table_exists(ht, 1));

  hash_table_insert(ht, 1);
  hash_table_insert(ht, 2);
  hash_table_insert(ht, 3);
  assert(hash_table_exists(ht, 2));
  assert(hash_table_remove(ht, 2));
  assert(!hash_table_exists(ht, 2));

  hash_table_destroy(ht);
}

int main() {
  test_operations_empty_hash_table();
  test_insert();
}
