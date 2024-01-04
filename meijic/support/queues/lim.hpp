#pragma once
#include <cstdlib>
#include <deque>
#include <meijic/support/lock/spinlock.hpp>
#include <meijic/support/semaphore.hpp>
#include <wheels/intrusive/forward_list.hpp>
namespace support::queue {
template <typename T> class MPMCLimitedQueue {
public:
  explicit MPMCLimitedQueue(size_t cap) : produce_(cap), consume_(0) {}

  void Put(T *obj) {
    Token t(std::move(produce_.Acquire()));

    {
      SpinLock::Guard guard{spinlock_};
      buff_.PushFront(obj);
    }

    consume_.Release(std::move(t));
  };

  T *Take() {
    Token t(std::move(consume_.Acquire()));
    T *obj;

    {
      SpinLock::Guard guard{spinlock_};
      obj = buff_.PopFront();
    }

    produce_.Release(std::move(t));
    return obj;
  };

private:
  // Tags
  struct SomeTag;

private:
  using Token = typename support::Semaphore<T *>::Token;
  Semaphore<T *> produce_;
  Semaphore<T *> consume_;
  SpinLock spinlock_;
  wheels::IntrusiveForwardList<T> buff_;
};
} // namespace support::queue
