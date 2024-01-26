#pragma once
#include <twist/ed/stdlike/atomic.hpp>
namespace support {
class SpinLock {
public:
  class Guard {
    friend class SpinLock;

  public:
    explicit Guard(SpinLock &host) : host_(host) { host_.Acquire(this); }

    ~Guard() { host_.Release(this); }

  private:
    SpinLock &host_;
    twist::ed::stdlike::atomic<Guard *> next_{nullptr};
    twist::ed::stdlike::atomic<bool> is_owner_{false};
  };

  SpinLock(){};

private:
  void Acquire(Guard *waiter) {
    auto prev_tail = tail_.exchange(waiter);
    if (IsChainLeader(prev_tail)) {
      return;
    }
    CompleteLink(prev_tail, waiter);
    WaitForAccess(waiter);
  }

  void Release(Guard *owner) {
    if (HaveNextWaiter(owner)) {
      WaitForLink(owner);
      SignalToNext(owner);
    }
  }

  void WaitForAccess(Guard *waiter) {
    while (!waiter->is_owner_.load(std::memory_order_acquire)) {
    }
  }

  void CompleteLink(Guard *prev, Guard *next) {
    prev->next_.store(next, std::memory_order_release);
  }

  bool IsChainLeader(Guard *prev) { return prev == nullptr; }

  bool HaveNextWaiter(Guard *owner) {
    return !tail_.compare_exchange_weak(owner, nullptr);
  }

  void WaitForLink(Guard *owner) {
    while (owner->next_.load(std::memory_order_acquire) == nullptr) {
    }
  }

  void SignalToNext(Guard *owner) {
    owner->next_.load(std::memory_order_acquire)
        ->is_owner_.store(true, std::memory_order_release);
  }

private:
  twist::ed::stdlike::atomic<Guard *> tail_{nullptr};
};
} // namespace support
