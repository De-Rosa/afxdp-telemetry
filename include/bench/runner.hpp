#ifndef RUNNER
#define RUNNER

#include "bench/stats.hpp"
#include "workloads/config.hpp"
#include "workloads/consumers.hpp"
#include "workloads/pin.hpp"
#include "workloads/producers.hpp"
#include <thread>
#include <vector>

namespace bench {
template <typename Queue> void run_saturated(Queue &q) {
  std::vector<double> latencies;
  bool order_valid = true;
  latencies.reserve(workloads::NUM_ITEMS - workloads::NUM_ITEMS_DISCARD);

  std::thread producer(workloads::saturated_producer<Queue>, std::ref(q));
  std::thread consumer(workloads::consumer<Queue>, std::ref(q),
                       std::ref(latencies), std::ref(order_valid));
  workloads::pin_to_core(producer, 2);
  workloads::pin_to_core(consumer, 3);
  producer.join();
  consumer.join();

  bench::report("saturated run", latencies, order_valid);
}

template <typename Queue>
void run_paced(Queue &q, std::chrono::nanoseconds interval) {
  std::vector<double> latencies;
  bool order_valid = true;
  latencies.reserve(workloads::NUM_ITEMS - workloads::NUM_ITEMS_DISCARD);

  std::thread producer(workloads::paced_producer<Queue>, std::ref(q), interval);
  std::thread consumer(workloads::consumer<Queue>, std::ref(q),
                       std::ref(latencies), std::ref(order_valid));
  workloads::pin_to_core(producer, 2);
  workloads::pin_to_core(consumer, 3);
  producer.join();
  consumer.join();

  bench::report("paced run", latencies, order_valid);
}
} // namespace bench

#endif // !RUNNER
