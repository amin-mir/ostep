#include <stdbool.h>

typedef struct _Node {
  int val;
  struct _Node *next;
} Node;

typedef struct {
  Node *head;
} LinkedList;

void sll_init(LinkedList *sll);
void sll_add(LinkedList *sll, int val);
bool sll_exists(LinkedList *sll, int val);
bool sll_remove(LinkedList *sll, int val);
void sll_clear(LinkedList *sll);
