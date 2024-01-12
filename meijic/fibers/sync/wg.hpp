#pragma once

#include <meijic/fibers/sync/event.hpp>
#include <twist/ed/std/atomic.hpp>

namespace fibers::sync {

class WaitGroup {
public:
  void Add(size_t delta) {
    counter_.fetch_add(delta, std::memory_order_release);
  }

  void Done() {
    if (counter_.fetch_sub(1, std::memory_order_acquire) == 1) {
      event_.Fire();
    }
  }

  void Wait() { event_.Wait(); }

private:
  twist::ed::std::atomic<size_t> counter_{0};
  Event event_;
};

} // namespace fibers::sync
