#pragma once
#include <cstdint>
#include <chime/fibers/awaiter.hpp>
#include <twist/ed/std/atomic.hpp>
namespace support::queue {
using IChainAwaiter = fibers::IChainAwaiter;

class AwaitersList {
public:
  bool TryAdd(IChainAwaiter *waiter) {
    while (true) {
      IChainAwaiter *head = head_.load();

      if (CheckState(head)) {
        return false;
      }

      waiter->next_ = head;
      if (head_.compare_exchange_weak(head, waiter)) {
        return true;
      }
    }
  }

  IChainAwaiter *Close() { return head_.exchange(ClosedState()); }

  bool IsClosed() { return CheckState(head_.load()); }

private:
  // Gratitude to Lipovsky Sealable Queue
  IChainAwaiter *ClosedState() { return (IChainAwaiter *)1; }

  bool CheckState(IChainAwaiter *state) { return (uintptr_t)state == 1; }

private:
  twist::ed::std::atomic<IChainAwaiter *> head_{nullptr};
};
} // namespace support::queue