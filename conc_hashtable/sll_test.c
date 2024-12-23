#include <assert.h>

#include "sll.h"
#include "utils.h"

static void test_operations_empty_list() {
  TEST;
  LinkedList ll;
  sll_init(&ll);

  assert(!sll_exists(&ll, 2));
  assert(!sll_remove(&ll, 10));
  sll_clear(&ll);
}

static void test_add() {
  TEST;
  LinkedList ll;
  sll_init(&ll);

  sll_add(&ll, 1);
  sll_add(&ll, 2);
  sll_add(&ll, 3);
  sll_add(&ll, 1);

  /* Non-existing element */
  assert(!sll_exists(&ll, 0));

  assert(sll_exists(&ll, 1));
  assert(sll_remove(&ll, 1));
  /* Another 1 still exists even after removing one of them */
  assert(sll_exists(&ll, 1));
  assert(sll_remove(&ll, 1));
  /* No 1 exists at this point */
  assert(!sll_exists(&ll, 1));

  assert(sll_remove(&ll, 2));
  /* 2 doesn't exist anymore */
  assert(!sll_remove(&ll, 2));
  assert(!sll_exists(&ll, 2));

  /* Remove all elements from the list */
  assert(sll_remove(&ll, 3));
  assert(!sll_exists(&ll, 1));
  assert(!sll_exists(&ll, 3));

  /* Should be able to work with the list after it's been cleaned up */
  sll_add(&ll, 10);
  assert(sll_exists(&ll, 10));

  sll_clear(&ll);
}

static void test_clear() {
  TEST;
  LinkedList ll;
  sll_init(&ll);

  sll_add(&ll, 1);
  sll_add(&ll, 2);
  sll_add(&ll, 3);
  sll_add(&ll, 4);

  sll_clear(&ll);

  assert(!sll_exists(&ll, 1));
  assert(!sll_exists(&ll, 4));
}

int main() {
  test_operations_empty_list();
  test_add();
  test_clear();
}
