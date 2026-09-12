#include <iostream>
#include "packet.hpp"
#include "queues/mutex_queue.hpp"

constexpr int PACKET_COUNT{100};

int main (int argc, char *argv[]) {
  mutex_queue<packet> queue;

  std::cout << "hello world\n" << PACKET_COUNT;
  return 0;
}
