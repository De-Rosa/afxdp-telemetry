#include "bench/runner.hpp"
#include "packet.hpp"
#include "queues/mutex_queue.hpp"
#include "queues/spsc_queue.hpp"
#include "workloads/config.hpp"
#include <cstdio>
#include <cstring>

int main(int argc, char *argv[]) {
  /*std::printf("-- mutex queue --\n");
  {
    mutex_queue<packet> q;
    bench::run_saturated(q);
  }
  {
    mutex_queue<packet> q;
    bench::run_paced(q, workloads::INTERVAL);
  }*/

  std::printf("-- spsc queue --\n");
  {
    spsc_queue<packet, workloads::CAPACITY> q;
    bench::run_saturated(q);
  }
  {
    spsc_queue<packet, workloads::CAPACITY> q;
    bench::run_paced(q, workloads::INTERVAL);
  }
  return 0;
}
