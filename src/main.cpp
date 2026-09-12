#include <chrono>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <thread>
#include "packet.hpp"
#include "queues/mutex_queue.hpp"
#include "queues/spsc_queue.hpp"

constexpr size_t CAPACITY = 1 << 16; 
constexpr size_t NUM_ITEMS = 1'000'000;
// warmup for caches/branch predictor
constexpr size_t NUM_ITEMS_DISCARD = 10'000;

template <typename queue>
struct queue_io;

// spsc
template <typename T, size_t capacity>
struct queue_io<spsc_queue<T, capacity>> {
  using spsc = spsc_queue<T, capacity>;

  static void push(spsc& q, const T& val) {
    while (!q.push(val)) {
      // spin instead of sleep until slot frees
    }
  }
  static void pop(spsc& q, T& val_out) {
    while (!q.pop(val_out)) {
      // spin instead of sleep
    }
  }
};

// mutex
template <typename T>
struct queue_io<mutex_queue<T>> {
  using mutex_q = mutex_queue<T>;

  static void push(mutex_q& q, const T& val) {
    q.push(val);
  }
  static void pop(mutex_q& q, T& val_out) {
    val_out = q.wait_pop();
  }
};

template <typename Queue>
void producer_run(Queue& q) {
  for (size_t i = 0; i < NUM_ITEMS; ++i) {
    packet p{i, std::chrono::steady_clock::now()};
    queue_io<Queue>::push(q, p);
  }
}

template <typename Queue>
void consumer_run(Queue& q, std::vector<double>& lat_out, bool& order_valid) {
  order_valid = true;
  for (size_t i = 0; i < NUM_ITEMS; ++i) {
    packet p;
    queue_io<Queue>::pop(q, p);

    if (p.id != i) order_valid = false;

    // get latency
    auto now = std::chrono::steady_clock::now();
    if (i < NUM_ITEMS_DISCARD) continue;
    auto lat = std::chrono::duration<double, std::micro>(now - p.timestamp).count();
    lat_out.push_back(lat);
  }
}

double percentile(const std::vector<double>& sorted, double p) {
  size_t index = static_cast<size_t>(p * sorted.size());
  return sorted[std::min(index, sorted.size() - 1)];
}

void print_sorted_latencies(const std::vector<double>& latencies) {
  std::printf("p50: %.3f us\n", percentile(latencies, 0.50));
  std::printf("p99: %.3f us\n", percentile(latencies, 0.99));
  std::printf("p99.9: %.3f us\n", percentile(latencies, 0.999));
  std::printf("max: %.3f us\n", latencies.back());
}

template <typename Queue>
void run(Queue& q) {
  std::vector<double> latencies;
  bool order_valid = true;

  latencies.reserve(NUM_ITEMS - NUM_ITEMS_DISCARD);

  std::thread producer(producer_run<Queue>, std::ref(q));
  std::thread consumer(consumer_run<Queue>, std::ref(q),
      std::ref(latencies), std::ref(order_valid));

  producer.join();
  consumer.join();

  std::printf("order valid?: %s\n", order_valid ? "yes" : "no");
  std::sort(latencies.begin(), latencies.end());
  print_sorted_latencies(latencies);
}

int main (int argc, char *argv[]) {
  std::printf("mutex queue\n");
  mutex_queue<packet> q;
  run(q);

  std::printf("spsc queue\n");
  spsc_queue<packet, CAPACITY> sq;
  run(sq);
  return 0;
}
