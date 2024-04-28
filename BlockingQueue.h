#include <assert.h>

#include <condition_variable>
#include <deque>
#include <mutex>

namespace ccutil {

template <typename T>
class BlockingQueue {
  typedef std::deque<T> Deque;
  typedef std::mutex mutex;
  typedef std::lock_guard<mutex> LockGuard;
  typedef std::unique_lock<mutex> UniqueLock;
  typedef std::condition_variable condition_variable;

 public:
  BlockingQueue() = default;

  void put(T&& x) {
    {
      LockGuard lock(mutex_);
      queue_.push_back(std::move(x));
    }
    not_empty_.notify_one();
  }

  T take() {
    UniqueLock lock(mutex_);
    not_empty_.wait(lock, [this] { return !queue_.empty(); });
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
  }

  size_t size() const {
    LockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable mutex mutex_;
  condition_variable not_empty_;
  Deque queue_;
};
}  // namespace ccutil