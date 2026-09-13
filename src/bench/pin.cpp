#include "workloads/pin.hpp"

// pthread_setaffinity_np only works on linux
#ifdef __linux__
#include <cstdio>
#include <cstring>
#include <pthread.h>

void workloads::pin_to_core(std::thread &thr, int core) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);

  // add requested core
  CPU_SET(core, &cpuset);

  int result =
      pthread_setaffinity_np(thr.native_handle(), sizeof(cpu_set_t), &cpuset);
  if (result != 0) {
    std::printf("failed to pin thread to cpu %d: %s\n", core,
                std::strerror(result));
  }
}
#else
void workloads::pin_to_core(std::thread &thr, int core) {}
#endif // __linux__
