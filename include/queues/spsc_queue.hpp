#ifndef SPSC_QUEUE
#define SPSC_QUEUE

#include <array>
#include <atomic>

namespace internal {
// https://en.cppreference.com/cpp/thread/hardware_destructive_interference_size
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_destructive_interference_size;
#else
constexpr size_t hardware_destructive_interference_size = 64;
#endif // __cpp_lib_hardware_interference_size

constexpr bool is_power_of_two(size_t n) { return (n & (n - 1)) == 0; }
} // namespace internal


// align to prevent false sharing
constexpr size_t line{internal::hardware_destructive_interference_size};

template <typename T, size_t capacity> class spsc_queue {
  static_assert(capacity > 0 && internal::is_power_of_two(capacity),
                "capacity must be a power of two");

private:
  static constexpr size_t _mask{capacity - 1};

  std::array<T, capacity> _buf;

  alignas(line) std::atomic<std::size_t> _head{0};
  alignas(line) std::atomic<std::size_t> _tail{0};

  // cache index instead of loading every push/pop call
  alignas(line) size_t _tail_cache{0};
  alignas(line) size_t _head_cache{0};

public:
  bool push(const T &val);
  bool push(T &&val);

  bool pop(T &val_out);
};

template <typename T, size_t capacity>
bool spsc_queue<T, capacity>::push(const T &val) {
  auto head = _head.load(std::memory_order_relaxed);
  auto next = head + 1;

  // producer's view of head is always accurate so will only
  // be stale in thinking that its full
  if (next == _tail_cache) {
    _tail_cache = _tail.load(std::memory_order_acquire);
    // is actually full
    if (next == _tail_cache)
      return false;
  };

  // using mask to wrap to capacity
  _buf[head & _mask] = val;

  // need to release head to consumers
  _head.store(next, std::memory_order_release);
  return true;
}

template <typename T, size_t capacity>
bool spsc_queue<T, capacity>::push(T &&val) {
  auto head = _head.load(std::memory_order_relaxed);
  auto next = head + 1;

  // we leave 1 to distinguish full from empty
  if (next == _tail_cache) {
    _tail_cache = _tail.load(std::memory_order_acquire);
    // is actually full
    if (next == _tail_cache)
      return false;
  };

  // using mask to wrap to capacity
  _buf[head & _mask] = std::move(val);

  _head.store(next, std::memory_order_release);
  return true;
}

template <typename T, size_t capacity>
bool spsc_queue<T, capacity>::pop(T &val_out) {
  auto tail = _tail.load(std::memory_order_relaxed);

  // head cache will only lag behind, so cannot be 'incorrect' if
  // not empty
  if (tail == _head_cache) {
    _head_cache = _head.load(std::memory_order_acquire);
    if (tail == _head_cache)
      return false;
  }

  val_out = _buf[tail & _mask];

  _tail.store(tail + 1, std::memory_order_release);
  return true;
}

#endif // !SPSC_QUEUE
