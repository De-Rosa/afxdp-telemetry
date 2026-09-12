#ifndef MUTEX_QUEUE
#define MUTEX_QUEUE

#include <queue>
#include <optional>
#include <mutex>
#include <condition_variable>

struct packet;

template <typename T>
class mutex_queue {
private:
  std::queue<T> _queue;
  mutable std::mutex _mutex;
  std::condition_variable _cv;
public:
  void push(const T& val);
  void push(T&& val);

  std::optional<T> try_pop(void);
  T wait_pop(void);

  void reserve(const size_t size) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.reserve(size);
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.size();
  }
};

template <typename T> void mutex_queue<T>::push(const T& value) {
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(value);
  }
  _cv.notify_one();
}

template <typename T> void mutex_queue<T>::push(T&& value) {
  {
    std::lock_guard<std::mutex> lock(_mutex);
    // converted to a l-value so need to move again
    _queue.push(std::move(value));
  }
  _cv.notify_one();
}

template <typename T> std::optional<T> mutex_queue<T>::try_pop(void) {
  std::lock_guard<std::mutex> lock(_mutex);
  if (_queue.empty()) return std::nullopt;

  T value = std::move(_queue.front());
  _queue.pop();

  return value;
}

template <typename T>T mutex_queue<T>::wait_pop(void) {
  std::unique_lock<std::mutex> lock(_mutex);

  _cv.wait(lock, [this] {
      return !_queue.empty();
      });

  T value = std::move(_queue.front());
  _queue.pop();

  return value;
}

#endif // !MUTEX_QUEUE
