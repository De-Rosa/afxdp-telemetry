#include "bench/stats.hpp"

double bench::percentile(const std::vector<double> &sorted, double p) {
  size_t index = static_cast<size_t>(p * sorted.size());
  return sorted[std::min(index, sorted.size() - 1)];
}

void bench::print_sorted_latencies(const std::vector<double> &latencies) {
  std::printf("p50: %.3f us\n", percentile(latencies, 0.50));
  std::printf("p99: %.3f us\n", percentile(latencies, 0.99));
  std::printf("p99.9: %.3f us\n", percentile(latencies, 0.999));
  std::printf("max: %.3f us\n", latencies.back());
}

void bench::report(const char *label, std::vector<double> &latencies,
                   bool order_valid) {
  std::printf("[%s] order valid?: %s\n", label, order_valid ? "yes" : "no");
  std::sort(latencies.begin(), latencies.end());
  print_sorted_latencies(latencies);
}
