#ifndef STATS
#define STATS

#include <vector>

namespace bench {
double percentile(const std::vector<double> &sorted, double p);
void print_sorted_latencies(const std::vector<double> &latencies);
void report(const char *label, std::vector<double> &latencies,
            bool order_valid);
} // namespace bench

#endif // !STATS
