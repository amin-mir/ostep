#include <assert.h>

#include "seq_linked_list.h"
#include "utils.h"

static void test_operations_empty_list() {
  TEST;
  LinkedList ll;
  linked_list_init(&ll);

  assert(!linked_list_exists(&ll, 2));
  assert(!linked_list_remove(&ll, 10));
  linked_list_clear(&ll);
}

static void test_add() {
  TEST;
  LinkedList ll;
  linked_list_init(&ll);

  linked_list_add(&ll, 1);
  linked_list_add(&ll, 2);
  linked_list_add(&ll, 3);
  linked_list_add(&ll, 1);

  /* Non-existing element */
  assert(!linked_list_exists(&ll, 0));

  assert(linked_list_exists(&ll, 1));
  assert(linked_list_remove(&ll, 1));
  /* Another 1 still exists even after removing one of them */
  assert(linked_list_exists(&ll, 1));
  assert(linked_list_remove(&ll, 1));
  /* No 1 exists at this point */
  assert(!linked_list_exists(&ll, 1));

  assert(linked_list_remove(&ll, 2));
  /* 2 doesn't exist anymore */
  assert(!linked_list_remove(&ll, 2));
  assert(!linked_list_exists(&ll, 2));

  /* Remove all elements from the list */
  assert(linked_list_remove(&ll, 3));
  assert(!linked_list_exists(&ll, 1));
  assert(!linked_list_exists(&ll, 3));

  /* Should be able to work with the list after it's been cleaned up */
  linked_list_add(&ll, 10);
  assert(linked_list_exists(&ll, 10));

  linked_list_clear(&ll);
}

static void test_clear() {
  TEST;
  LinkedList ll;
  linked_list_init(&ll);

  linked_list_add(&ll, 1);
  linked_list_add(&ll, 2);
  linked_list_add(&ll, 3);
  linked_list_add(&ll, 4);

  linked_list_clear(&ll);

  assert(!linked_list_exists(&ll, 1));
  assert(!linked_list_exists(&ll, 4));
}

int main() {
  test_operations_empty_list();
  test_add();
  test_clear();
}
