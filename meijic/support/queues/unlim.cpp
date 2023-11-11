#include <meijic/support/queues/unlim.hpp>
namespace sup::queue {

template <typename T> bool sup::queue::MPMCUnlimitedQueue<T>::Put(T val) {
  {
    std::lock_guard<std::mutex> lg(mutex_);
    if (!is_open_) {
      return false;
    }
    q_.push(std::move(val));
  }
  cond_.notify_one();
  return true;
}

template <typename T>
std::optional<T> sup::queue::MPMCUnlimitedQueue<T>::Take() {
  std::optional<T> res;
  {
    std::unique_lock<std::mutex> ul(mutex_);
    while (q_.empty()) {
      if (!is_open_) {
        return std::nullopt;
      }
      cond_.wait(ul);
    }
    res = std::make_optional<T>(std::move(q_.front()));
    q_.pop();
  }
  return res;
}

template <typename T> void sup::queue::MPMCUnlimitedQueue<T>::Close() {
  {
    std::lock_guard<std::mutex> lg(mutex_);
    is_open_ = false;
  }
  cond_.notify_all();
}
} // namespace supp::queue