#pragma once

#include <cstdlib>
#include <twist/ed/stdlike/condition_variable.hpp>
#include <twist/ed/stdlike/mutex.hpp>

namespace support {

// Group for wild threads

class WaitGroup {
public:
  void Add(size_t delta) {
    std::lock_guard<Mutex> guard{mutex_};
    counter_ += delta;
  }

  void Done() {
    std::lock_guard<Mutex> guard{mutex_};
    --counter_;
    if (counter_ == 0) {
      if (sleeps_ > 1) {
        cond_.notify_all();
      }
      if (sleeps_ == 1) {
        cond_.notify_one();
      }
    }
  }

  void Wait() {
    {
      std::unique_lock<Mutex> guard{mutex_};
      ++sleeps_;

      cond_.wait(guard, [&]() { return counter_ == 0; });

      --sleeps_;
    }
  }

private:
  using Mutex = twist::ed::stdlike::mutex;
  Mutex mutex_;
  twist::ed::stdlike::condition_variable cond_;
  size_t counter_{0}, sleeps_{0};
};
} // namespace support
