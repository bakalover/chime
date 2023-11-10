#include <meijic/support/queues/lim.hpp>
namespace supp::queue {
template <typename T> void MPMCLimitedQueue<T>::Put(T obj) {
  Token t(std::move(produce_.Acquire()));

  {
    SpinLock::Guard guard(spinlock_);
    buff_.push_front(std::move(obj));
  }

  consume_.Release(std::move(t));
}

template <typename T> T MPMCLimitedQueue<T>::Take() {
  Token t(std::move(consume_.Acquire()));
  T obj;

  {
    SpinLock::Guard guard{spinlock_};
    obj = std::move(buff_.back());
    buff_.pop_back();
  }

  produce_.Release(std::move(t));
  return obj;
}
} // namespace supp::queue