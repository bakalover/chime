#pragma once

#include <chime/fibers/awaiter.hpp>
#include <cstdint>
#include <twist/ed/stdlike/atomic.hpp>

namespace fibers::internal {

class AwaitList {
public:
  bool TryAdd(ChainAwaiterBase *waiter) {
    while (true) {
      ChainAwaiterBase *head = head_.load();

      if (CheckState(head)) {
        return false;
      }

      waiter->next_ = head;
      if (head_.compare_exchange_weak(head, waiter)) {
        return true;
      }
    }
  }

  ChainAwaiterBase *Close() { return head_.exchange(ClosedState()); }

  bool IsClosed() { return CheckState(head_.load()); }

private:
  ChainAwaiterBase *ClosedState() { return (ChainAwaiterBase *)1; }

  bool CheckState(ChainAwaiterBase *state) { return (uintptr_t)state == 1; }

private:
  twist::ed::stdlike::atomic<ChainAwaiterBase *> head_{nullptr};
};
} // namespace fibers::internal