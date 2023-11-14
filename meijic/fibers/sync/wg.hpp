#pragma once
#include <atomic>
#include <meijic/fibers/sync/event.hpp>
namespace fib::sync {

// https://gobyexample.com/waitgroups

class WaitGroup {
public:
  void Add(size_t delta) {
    counter_.fetch_add(delta, std::memory_order_release);
  }

  void Done() {
    if (counter_.fetch_sub(1, std::memory_order_release) == 1) {
      event_.Fire();
    }
  }

  void Wait() { event_.Wait(); }

private:
  std::atomic<size_t> counter_{0};
  Event event_;
};

} // namespace fib::sync
