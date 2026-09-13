#ifndef CONFIG
#define CONFIG

#include <chrono>
#include <cstddef>

namespace workloads {
using namespace std::chrono_literals;

constexpr size_t CAPACITY = 1 << 16;
constexpr size_t NUM_ITEMS = 1'000'000;
// warmup for caches/branch predictor
constexpr size_t NUM_ITEMS_DISCARD = 10'000;
constexpr std::chrono::nanoseconds INTERVAL = 10us;
} // namespace workloads

#endif // !CONFIG
