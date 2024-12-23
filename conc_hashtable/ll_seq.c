#include <stdlib.h>

#include "ll_seq.h"
#include "utils.h"

void linked_list_init(LinkedList *ll) { ll->head = NULL; }

void linked_list_add(LinkedList *ll, int val) {
  Node *n = (Node *)Malloc(sizeof(Node));
  n->val = val;
  n->next = NULL;

  if (ll->head) {
    n->next = ll->head;
  }
  ll->head = n;
}

bool linked_list_exists(LinkedList *ll, int val) {
  Node *n = ll->head;
  while (n) {
    if (n->val == val) {
      return true;
    }
    n = n->next;
  }
  return false;
}

/* Returns 1 if value was removed or 0 otherwise. */
static Node *linked_list_soft_remove(LinkedList *ll, int val) {
  Node *head = ll->head;
  if (!head) {
    return NULL;
  }

  /* Handle the case where head of list has to be removed */
  // TODO: fix memory leak.
  if (head->val == val) {
    ll->head = head->next;
    return head;
  }

  Node *prev = head;
  Node *cur = head->next;
  while (cur) {
    if (cur->val == val) {
      prev->next = cur->next;
      return cur;
    }
    prev = cur;
    cur = cur->next;
  }
  return NULL;
}

/* Soft removing only removes an element from the list and returns a reference
 to it. The allocated memory has to be freed by the caller. */
bool linked_list_remove(LinkedList *ll, int val) {
  Node *n = linked_list_soft_remove(ll, val);
  if (n) {
    free(n);
    return true;
  }
  return false;
}

void linked_list_clear(LinkedList *ll) {
  Node *n = ll->head;
  while (n) {
    Node *tmp = n->next;
    free(n);
    n = tmp;
  }
  ll->head = NULL;
}
