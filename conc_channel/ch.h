#include <pthread.h>
#include <stdbool.h>

constexpr size_t max_ch_size = 100;

typedef struct {
  pthread_cond_t cv_fill;
  pthread_cond_t cv_empty;
  pthread_mutex_t lock;
  size_t data_len;
  size_t data_wi;
  size_t data_ri;
  size_t data_cap;
  bool closed;
  int data[];
} Channel;

void print_ch_info(Channel *ch);
Channel *ch_create(size_t size);
void ch_destroy(Channel *ch);
int ch_send(Channel *ch, int val);
int ch_recv(Channel *ch, int *val);
void ch_close(Channel *ch);
