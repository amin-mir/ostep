#include <pthread.h>
#include <stdlib.h>

#include "../utils.h"
#include "ch.h"

void print_ch_info(Channel *ch) {
  printf("channel info:\ncap: %ld, len: %ld\nwrite idx: %ld, read idx: %ld, "
         "data[0]: %d\n\n",
         ch->data_cap, ch->data_len, ch->data_wi, ch->data_ri, ch->data[0]);
}

/* Allocated memory by ch_created will be freed when ch_destroy is called. */
Channel *ch_create(size_t size) {
  if (size > max_ch_size)
    return nullptr;

  Channel *ch = Malloc(sizeof(Channel) + sizeof(int) * size);
  if (ch == nullptr)
    return nullptr;

  Pthread_cond_init(&ch->cv_fill);
  Pthread_cond_init(&ch->cv_empty);
  Pthread_mutex_init(&ch->lock);

  ch->data_cap = size;
  ch->data_len = 0;
  ch->data_wi = 0;
  ch->data_ri = 0;
  ch->closed = false;
  for (size_t i = 0; i < size; i++) {
    ch->data[i] = 0;
  }

  return ch;
}

/**
 * No thread should be waiting on this channel when the destrcutor is called.
 * It's undefined behavior to call Pthread_con_destroy if a thread is waiting
 * on the ConditionVariable.
 */
void ch_destroy(Channel *ch) {
  Pthread_cond_destroy(&ch->cv_fill);
  Pthread_cond_destroy(&ch->cv_empty);
  Pthread_mutex_destroy(&ch->lock);
  free(ch);
}

void ch_close(Channel *ch) {
  Pthread_mutex_lock(&ch->lock);
  ch->closed = true;
  Pthread_cond_broadcast(&ch->cv_empty);
  Pthread_cond_broadcast(&ch->cv_fill);
  Pthread_mutex_unlock(&ch->lock);
}

/* Return 0 on success, and -1 when channel is closed. */
int ch_send(Channel *ch, int val) {
  Pthread_mutex_lock(&ch->lock);

  /* Avoid writing to a closed channel. */
  if (ch->closed) {
    Pthread_mutex_unlock(&ch->lock);
    return -1;
  }

  while (ch->data_len == ch->data_cap) {
    Pthread_cond_wait(&ch->cv_empty, &ch->lock);

    /**
     * Wake up could be due to a broadcast indicating channel closure.
     * When we reach here, lock is acquired by us, so make sure to release it.
     */
    if (ch->closed) {
      Pthread_mutex_unlock(&ch->lock);
      return -1;
    }
  }

  /* We will have the lock when we exit the while-loop. */
  ch->data[ch->data_wi] = val;
  ch->data_wi++;
  ch->data_wi = ch->data_wi % ch->data_cap;

  /**
   * When data_len becomes 1, it means channel was empty before this write. So
   * there could potentially be consumers waiting on it. We, therefore, wake one
   * of them.
   */
  ch->data_len++;
  if (ch->data_len == 1)
    Pthread_cond_signal(&ch->cv_fill);

  Pthread_mutex_unlock(&ch->lock);
  return 0;
}

/**
 * Return 0 on success, and -1 when channel is empty & closed. ch_recv will
 * consume all elements from a closed channel and then returns -1.
 */
int ch_recv(Channel *ch, int *val) {
  Pthread_mutex_lock(&ch->lock);

  while (ch->data_len == 0) {
    /* We shouldn't wait if channel is empty & closed. */
    if (ch->closed) {
      Pthread_mutex_unlock(&ch->lock);
      return -1;
    }

    /* Wake up could be due to a broadcast indicating channel closure. */
    Pthread_cond_wait(&ch->cv_fill, &ch->lock);
  }

  /* We will have the lock when we exit the while-loop. */
  *val = ch->data[ch->data_ri];
  ch->data_ri++;
  ch->data_ri = ch->data_ri % ch->data_cap;

  /**
   * When data_len is equal to data_cap, it means channel was full prior to this
   * read. So there could potentially be producers waiting to write values into
   * the channel. We, therefore, wake one of them if channel is not closed.
   */
  size_t old_len = ch->data_len--;
  if (old_len == ch->data_cap && !ch->closed)
    Pthread_cond_signal(&ch->cv_empty);

  Pthread_mutex_unlock(&ch->lock);
  return 0;
}
