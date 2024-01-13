#pragma once
#include <chime/fibers/awaiter.hpp>
#include <cstdint>
#include <twist/ed/std/atomic.hpp>
namespace support::queue {
using ChainAwaiterBase = fibers::ChainAwaiterBase;

class AwaitersList {
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
  // Gratitude to Lipovsky Sealable Queue
  ChainAwaiterBase *ClosedState() { return (ChainAwaiterBase *)1; }

  bool CheckState(ChainAwaiterBase *state) { return (uintptr_t)state == 1; }

private:
  twist::ed::std::atomic<ChainAwaiterBase *> head_{nullptr};
};
} // namespace support::queue