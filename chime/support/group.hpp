#pragma once

#include "twist/ed/stdlike/atomic.hpp"
#include <cstdlib>
#include <mutex>
#include <twist/ed/stdlike/condition_variable.hpp>
#include <twist/ed/stdlike/mutex.hpp>

namespace support {

// Group for wild threads

class WaitGroup {
public:
  void Add(size_t delta = 1) {
    // Fast
    counter_.fetch_add(delta);
  }

  void Done(size_t delta = 1) {
    if (counter_.fetch_sub(delta) == delta) {
      std::lock_guard guard{mutex_};
      cond_.notify_all();
    }
  }

  void Wait() {
    std::unique_lock guard{mutex_};
    while (counter_.load() > 0) {
      cond_.wait(guard);
    }
  }

private:
  twist::ed::stdlike::mutex mutex_;
  twist::ed::stdlike::condition_variable cond_;
  twist::ed::stdlike::atomic<size_t> counter_{0};
};
} // namespace support
