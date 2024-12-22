#include <stdbool.h>

typedef struct _Node {
  int val;
  struct _Node *next;
} Node;

typedef struct {
  Node *head;
} LinkedList;

void linked_list_init(LinkedList *ll);
void linked_list_add(LinkedList *ll, int val);
bool linked_list_exists(LinkedList *ll, int val);
bool linked_list_remove(LinkedList *ll, int val);
void linked_list_clear(LinkedList *ll);
