#include <stdbool.h>
#include <pthread.h>

typedef struct _Node {
  int val;
  struct _Node *next;
} Node;

typedef struct {
  pthread_mutex_t lock;
  Node *head;
} LinkedList;

void cll_init(LinkedList *cll);
bool cll_add_not_exists(LinkedList *cll, int val);
bool cll_exists(LinkedList *cll, int val);
bool cll_remove(LinkedList *cll, int val);
void cll_clear(LinkedList *cll);
