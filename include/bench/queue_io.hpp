#ifndef QUEUE_IO
#define QUEUE_IO

#include "queues/mutex_queue.hpp"
#include "queues/spsc_queue.hpp"
#include <cstddef>

template <typename queue> struct queue_io;

// spsc
template <typename T, size_t capacity>
struct queue_io<spsc_queue<T, capacity>> {
  using spsc = spsc_queue<T, capacity>;

  static void push(spsc &q, const T &val) {
    while (!q.push(val)) {
      // spin instead of sleep until slot frees
    }
  }
  static void pop(spsc &q, T &val_out) {
    while (!q.pop(val_out)) {
      // spin instead of sleep
    }
  }
};

// mutex
template <typename T> struct queue_io<mutex_queue<T>> {
  using mutex_q = mutex_queue<T>;

  static void push(mutex_q &q, const T &val) { q.push(val); }
  static void pop(mutex_q &q, T &val_out) { val_out = q.wait_pop(); }
};

#endif // !QUEUE_IO
