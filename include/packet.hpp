#ifndef PACKET
#define PACKET

#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <ctime>
#include <queue>

struct packet {
  uint64_t id;
  std::time_t timestamp;
};

#endif // PACKET
