#ifndef PACKET
#define PACKET

#include <chrono>
#include <cstdint>

struct packet {
  uint64_t id;
  std::chrono::steady_clock::time_point timestamp;
};

#endif // PACKET
