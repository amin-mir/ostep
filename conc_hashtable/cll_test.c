#include <assert.h>

#include "cll.h"
#include "utils.h"

static void test_operations_empty_list() {
  TEST;
  LinkedList ll;
  cll_init(&ll);

  assert(!cll_exists(&ll, 2));
  assert(!cll_remove(&ll, 10));
  cll_clear(&ll);
}

static void test_add_not_exists() {
  TEST;
  LinkedList ll;
  cll_init(&ll);

  assert(cll_add_not_exists(&ll, 1));
  assert(cll_add_not_exists(&ll, 2));
  assert(cll_add_not_exists(&ll, 3));
  assert(!cll_add_not_exists(&ll, 1));

  /* Non-existing element */
  assert(!cll_exists(&ll, 0));

  assert(cll_exists(&ll, 1));
  assert(cll_remove(&ll, 1));
  /* 1 already removed */
  assert(!cll_exists(&ll, 1));
  assert(!cll_remove(&ll, 1));

  assert(cll_remove(&ll, 2));
  /* 2 doesn't exist anymore */
  assert(!cll_remove(&ll, 2));
  assert(!cll_exists(&ll, 2));

  /* Remove all elements from the list */
  assert(cll_remove(&ll, 3));
  assert(!cll_exists(&ll, 1));
  assert(!cll_exists(&ll, 3));

  /* Should be able to work with the list after it's been cleaned up */
  cll_add_not_exists(&ll, 10);
  assert(cll_exists(&ll, 10));

  cll_clear(&ll);
}

static void test_clear() {
  TEST;
  LinkedList ll;
  cll_init(&ll);

  cll_add_not_exists(&ll, 1);
  cll_add_not_exists(&ll, 2);
  cll_add_not_exists(&ll, 3);
  cll_add_not_exists(&ll, 4);

  cll_clear(&ll);

  assert(!cll_exists(&ll, 1));
  assert(!cll_exists(&ll, 4));
}

int main() {
  test_operations_empty_list();
  test_add_not_exists();
  test_clear();
}
