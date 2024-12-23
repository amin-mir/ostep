#include <pthread.h>
#include <stddef.h>

#include "ll_seq.h"

typedef struct {
  pthread_mutex_t lock;
  size_t buckets;
  LinkedList lists[];
} HashTable;

HashTable *hash_table_new(size_t buckets);
void hash_table_destroy(HashTable *ht);
/* Returns true when key is inserted in the HashTable. When key is already
 * present in the map, false is returned */
bool hash_table_insert(HashTable *ht, int key);
bool hash_table_exists(HashTable *ht, int key);
bool hash_table_remove(HashTable *ht, int key);
