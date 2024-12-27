#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"
#include "ch.h"

constexpr size_t WRITE_CLOSER_LOOPS = 50000;
constexpr size_t NUM_THREADS = 4;

void *recv_unblock_send(void *arg) {
  Channel *ch = arg;
  int *res = Malloc(sizeof(int));
  assert(res != nullptr);
  assert(!ch_recv(ch, res));
  return res;
}

void test_recv_unblocks_send() {
  TEST;
  Channel *ch = ch_create(2);

  assert(!ch_send(ch, 10));
  assert(!ch_send(ch, 20));

  pthread_t tid;
  Pthread_create(&tid, nullptr, recv_unblock_send, ch);

  /* This would block until thread calls recv */
  assert(!ch_send(ch, 30));
  int *res;
  Pthread_join(tid, (void **)&res);
  assert(*res == 10);

  ch_destroy(ch);
  free(res);
}

void *send_unblock_recv(void *arg) {
  Channel *ch = arg;
  assert(!ch_send(ch, 10));
  return nullptr;
}

void test_send_unblocks_recv() {
  TEST;
  Channel *ch = ch_create(2);

  pthread_t tid;
  Pthread_create(&tid, nullptr, send_unblock_recv, ch);

  /**
   * This would block until thread calls send. We don't have to join the thread
   * as we're not interested in its return values. But the fact that ch_recv
   * unblocks is an indication that the child thread finished execution.
   */
  int res = -1;
  assert(!ch_recv(ch, &res));
  assert(res == 10);

  ch_destroy(ch);
}

void *send_closer(void *arg) {
  Channel *ch = arg;
  for (size_t i = 0; i < WRITE_CLOSER_LOOPS; i++) {
    assert(!ch_send(ch, i));
  }
  ch_close(ch);

  size_t *res = Malloc(sizeof(size_t));
  assert(res != nullptr);
  *res = WRITE_CLOSER_LOOPS;
  return res;
}

void *send_until_closed(void *arg) {
  Channel *ch = arg;

  size_t *cnt_send = Malloc(sizeof(size_t));
  assert(cnt_send != nullptr);
  *cnt_send = 0;

  while (ch_send(ch, 10) == 0) {
    (*cnt_send)++;
  }
  return cnt_send;
}

void *recv_until_closed(void *arg) {
  Channel *ch = arg;

  size_t *cnt_recv = Malloc(sizeof(size_t));
  assert(cnt_recv != nullptr);
  *cnt_recv = 0;

  int *res = Malloc(sizeof(int));
  assert(res != nullptr);

  while (ch_recv(ch, res) == 0) {
    (*cnt_recv)++;
  }
  return cnt_recv;
}

void test_close_channel() {
  TEST;
  Channel *ch = ch_create(2);

  size_t i;
  pthread_t send_tids[NUM_THREADS];
  for (i = 0; i < NUM_THREADS - 1; i++) {
    Pthread_create(&send_tids[i], NULL, send_until_closed, ch);
  }
  /* i will be the index of the last element of array. */
  Pthread_create(&send_tids[i], NULL, send_closer, ch);

  pthread_t recv_tids[NUM_THREADS];
  for (i = 0; i < NUM_THREADS; i++) {
    Pthread_create(&recv_tids[i], NULL, recv_until_closed, ch);
  }

  size_t cnt = 0;
  size_t *cnt_ptr = &cnt;
  size_t cnt_send = 0;
  for (i = 0; i < NUM_THREADS; i++) {
    Pthread_join(send_tids[i], (void **)&cnt_ptr);
    cnt_send += *cnt_ptr;
  }

  size_t cnt_recv = 0;
  for (i = 0; i < NUM_THREADS; i++) {
    Pthread_join(recv_tids[i], (void **)&cnt_ptr);
    cnt_recv += *cnt_ptr;
  }

  assert(cnt_send == cnt_recv);
}

int main() {
  test_recv_unblocks_send();
  test_send_unblocks_recv();
  test_close_channel();
}
