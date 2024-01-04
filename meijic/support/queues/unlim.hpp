#pragma once
#include <optional>
#include <twist/ed/std/condition_variable.hpp>
#include <twist/ed/std/mutex.hpp>
#include <wheels/intrusive/forward_list.hpp>
namespace support::queue {
template <typename T> class MPMCUnlimitedQueue {
public:
  bool Put(T *val) {
    {
      std::lock_guard<std::mutex> lg(mutex_);
      if (!is_open_) {
        return false;
      }
      queue_.PushBack(val);
    }
    cond_.notify_one();
    return true;
  }

  std::optional<T *> Take() {
    std::optional<T *> res;
    {
      std::unique_lock<std::mutex> ul(mutex_);
      while (queue_.IsEmpty()) {
        if (!is_open_) {
          return std::nullopt;
        }
        cond_.wait(ul);
      }
      res = std::make_optional<T *>(queue_.PopFront());
    }
    return res;
  }

  void Close() {
    {
      std::lock_guard<std::mutex> lg(mutex_);
      is_open_ = false;
    }
    cond_.notify_all();
  }

private:
  wheels::IntrusiveForwardList<T> queue_;
  twist::ed::std::mutex mutex_;
  twist::ed::std::condition_variable cond_;
  bool is_open_ = true;
};

} // namespace support::queue