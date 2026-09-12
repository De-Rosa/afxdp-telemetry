#ifndef SPSC_QUEUE
#define SPSC_QUEUE

#include <array>
#include <new>
#include <atomic>

constexpr bool is_power_of_two(size_t n) {
  return (n & (n - 1)) == 0;
}

// https://en.cppreference.com/cpp/thread/hardware_destructive_interference_size
#ifdef __cpp_lib_hardware_interference_size
  using std::hardware_destructive_interference_size;
#else
  constexpr size_t hardware_destructive_interference_size = 64;
#endif // __cpp_lib_hardware_interference_size

template <typename T, size_t capacity>
class spsc_queue {
static_assert(capacity > 0 && is_power_of_two(capacity), "capacity must be a power of two");

private:
  std::array<T, capacity> _buf;

  // align to prevent false sharing
  alignas(hardware_destructive_interference_size) std::atomic<std::size_t> _head{0};
  alignas(hardware_destructive_interference_size) std::atomic<std::size_t> _tail{0};

  // capacity is power of two
  inline size_t _wrap(size_t val) {
    return (val + 1) & (capacity - 1);
  }
public:
  bool push(const T& val);
  bool push(T&& val);

  bool pop(T& val_out);

  bool empty(void) const {
    // empty if tall == head
    auto tail = _tail.load(std::memory_order_relaxed);
    auto head = _head.load(std::memory_order_relaxed);
    return tail == head;
  }
};

template <typename T, size_t capacity> 
bool spsc_queue<T, capacity>::push(const T& val) {
  auto head = _head.load(std::memory_order_relaxed);
  auto next = _wrap(head);

  auto tail = _tail.load(std::memory_order_acquire);
  // we leave 1 to distinguish full from empty
  if (next == tail) return false;

  _buf[head] = val;

  // need to release head to consumers
  _head.store(next, std::memory_order_release);
  return true;
}

template <typename T, size_t capacity>
bool spsc_queue<T, capacity>::push(T&& val) {
  auto head = _head.load(std::memory_order_relaxed);
  auto next = _wrap(head); 

  auto tail = _tail.load(std::memory_order_acquire);
  // we leave 1 to distinguish full from empty
  if (next == tail) return false;

  _buf[head] = std::move(val);

  _head.store(next, std::memory_order_release);
  return true;
}

template <typename T, size_t capacity>
bool spsc_queue<T, capacity>::pop(T& val_out) {
  auto tail = _tail.load(std::memory_order_relaxed);

  // need to acquire head from producers
  auto head = _head.load(std::memory_order_acquire);
  // is empty
  if (tail == head) return false;

  val_out = _buf[tail];

  auto next = _wrap(tail);
  _tail.store(next, std::memory_order_release);
  return true;
}

#endif // !SPSC_QUEUE
