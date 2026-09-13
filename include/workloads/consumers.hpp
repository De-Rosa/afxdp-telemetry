#ifndef CONSUMERS
#define CONSUMERS

#include "bench/queue_io.hpp"
#include "packet.hpp"
#include "workloads/config.hpp"
#include <vector>

namespace workloads {
template <typename Queue>
void consumer(Queue &q, std::vector<double> &lat_out, bool &order_valid) {
  order_valid = true;
  for (size_t i = 0; i < workloads::NUM_ITEMS; ++i) {
    packet p;
    queue_io<Queue>::pop(q, p);

    if (p.id != i)
      order_valid = false;

    // get latency
    auto now = std::chrono::steady_clock::now();
    if (i < workloads::NUM_ITEMS_DISCARD)
      continue;
    auto lat =
        std::chrono::duration<double, std::micro>(now - p.timestamp).count();
    lat_out.push_back(lat);
  }
}
} // namespace workloads

#endif // !CONSUMERS
