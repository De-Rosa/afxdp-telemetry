#ifndef PRODUCERS
#define PRODUCERS

#include "bench/queue_io.hpp"
#include "packet.hpp"
#include "workloads/config.hpp"
#include <chrono>

namespace workloads {
// producer open-loop so faster than the consumer
// results dominated by queue waiting time
template <typename Queue> void saturated_producer(Queue &q) {
  for (size_t i = 0; i < workloads::NUM_ITEMS; ++i) {
    packet p{i, std::chrono::steady_clock::now()};
    queue_io<Queue>::push(q, p);
  }
}

// queue stays near-empty so latency reflects transit time
// rather than queue delay
template <typename Queue>
void paced_producer(Queue &q, std::chrono::nanoseconds interval) {
  auto send_next{std::chrono::steady_clock::now()};
  for (size_t i = 0; i < workloads::NUM_ITEMS; ++i) {
    packet p{i, std::chrono::steady_clock::now()};
    queue_io<Queue>::push(q, p);

    send_next += interval;
    while (std::chrono::steady_clock::now() < send_next) {
      // spin
    }
  }
}
} // namespace workloads

#endif // !PRODUCERS
