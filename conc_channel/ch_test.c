#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"
#include "ch.h"

constexpr size_t WRITE_CLOSER_LOOPS = 500000;

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

void *write_closer(void *arg) {
  Channel *ch = arg;
  for (size_t i = 0; i < WRITE_CLOSER_LOOPS; i++) {
    assert(!ch_send(ch, i));
  }
  ch_close(ch);

  int *res = Malloc(sizeof(int));
  assert(res != nullptr);
  *res = WRITE_CLOSER_LOOPS;
  return res;
}

void *send_until_closed(void *arg) { return nullptr; }

void *recv_until_closed(void *arg) { return nullptr; }

void test_close_channel() { TEST; }

int main() {
  test_recv_unblocks_send();
  test_send_unblocks_recv();
  test_close_channel();
}
