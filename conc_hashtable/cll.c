#include <stdlib.h>

#include "../utils.h"
#include "cll.h"

#define FILE_ID "conc"

void cll_init(LinkedList *cll) {
  cll->head = NULL;
  Pthread_mutex_init(&cll->lock);
}

/*
 * Only adds to the list of a matching val doesn't exist in the list. It
 * involves traversing the entire list to look for val. This is more efficient
 * than a combination of cll_exists and cll_add as it acquires the lock only a
 * single time.
 */
bool cll_add_not_exists(LinkedList *cll, int val) {
  Pthread_mutex_lock(&cll->lock);
  Node *n = cll->head;
  while (n) {
    if (n->val == val) {
      Pthread_mutex_unlock(&cll->lock);
      return false;
    }
    n = n->next;
  }

  n = (Node *)Malloc(sizeof(Node));
  n->val = val;
  n->next = NULL;

  if (cll->head) {
    n->next = cll->head;
  }
  cll->head = n;
  Pthread_mutex_unlock(&cll->lock);
  return true;
}

bool cll_exists(LinkedList *cll, int val) {
  bool exists = false;
  Pthread_mutex_lock(&cll->lock);
  Node *n = cll->head;
  while (n) {
    if (n->val == val) {
      exists = true;
      break;
    }
    n = n->next;
  }
  Pthread_mutex_unlock(&cll->lock);
  return exists;
}

/*
 * Soft removing only removes an element from the list and returns a reference
 * to it. The allocated memory has to be freed by the caller. Assumes lock is
 * already acquired.
 */
static Node *cll_soft_remove(LinkedList *cll, int val) {
  Node *head = cll->head;
  if (!head) {
    return NULL;
  }

  /* Handle the case where head of list has to be removed */
  if (head->val == val) {
    cll->head = head->next;
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

/* Returns true if value was removed or false otherwise. */
bool cll_remove(LinkedList *cll, int val) {
  bool removed = false;
  Pthread_mutex_lock(&cll->lock);
  Node *n = cll_soft_remove(cll, val);
  if (n) {
    free(n);
    removed = true;
  }
  Pthread_mutex_unlock(&cll->lock);
  return removed;
}

void cll_clear(LinkedList *cll) {
  Node *n = cll->head;
  while (n) {
    Node *tmp = n->next;
    free(n);
    n = tmp;
  }
  cll->head = NULL;
}
