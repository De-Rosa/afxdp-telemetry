#ifndef PACKET
#define PACKET

#include <cstdint>
#include <chrono>

struct packet {
  uint64_t id;
  std::chrono::steady_clock::time_point timestamp;
};

#endif // PACKET
