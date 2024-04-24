#include <assert.h>

#include <condition_variable>
#include <deque>
#include <mutex>

namespace ccutil {
using std::condition_variable;
using std::deque;
using std::lock_guard;
using std::mutex;
using std::unique_lock;
template <typename T>
class BlockingQueue {
 public:
  BlockingQueue() = default;

  void put(T&& x) {
    {
      lock_guard<mutex> lock(mutex_);
      queue_.push_back(std::move(x));
    }
    not_empty_.notify_one();
  }

  T take() {
    unique_lock<mutex> lock(mutex_);
    not_empty_.wait(lock, [this] { return !queue_.empty(); });
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
  }

  size_t Size() const {
    lock_guard<mutex> lock(mutex_);
    return queue_.size();
  }

 private:
  mutable mutex mutex_;
  condition_variable not_empty_;
  deque<T> queue_;
};
}  // namespace ccutil