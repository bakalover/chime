#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/handle.hpp>

namespace fib {

class Mutex {
private:
  struct LockAwaiter : IChainAwaiter {
    explicit LockAwaiter(Mutex &host) : host_(host) {}

    void AwaitSuspend(FiberHandle handle) override {
      handle_ = handle;
      if (host_.Acquire(this)) {
        handle_.Schedule();
      };
    }

    Mutex &host_;
  };

  using State = IChainAwaiter *;

public:
  void Lock() {
    LockAwaiter lock_awaiter{*this};
    if (Fiber::IsFiber()) {
      Fiber::Self()->Suspend(&lock_awaiter);
    }
  }

  void Unlock() {
    while (true) {
      // Prerequisite: head_ != nullptr =>
      // => head_ == (LockAwaiter*)1 aka SingleLocked or LockAwaiter* aka Queue
      State current_head = GetCurrent();

      assert(!IsUnlocked(current_head));

      if (IsSingleLocked(current_head)) {
        if (TryCommit(current_head, Unlocked(), std::memory_order::release)) {
          return;
        }
        continue;
      } else {
        if (TryCommit(current_head, current_head->next_,
                      std::memory_order::release)) {
          current_head->handle_.Schedule();
          return;
        }
        continue;
      }
    }
  }

private:
  bool Acquire(LockAwaiter *awaiter) {
    while (true) {
      State cur_head = head_.load(std::memory_order_acquire);
      if (IsUnlocked(cur_head)) {
        if (TryCommit(cur_head, SingleLocked(), std::memory_order::acquire)) {
          return true;
        }
        continue;
      } else {
        awaiter->next_ = cur_head;
        if (TryCommit(cur_head, awaiter, std::memory_order::release)) {
          return false;
        }
        continue;
      }
    }
  }

  bool TryCommit(State from, State to, std::memory_order mo) {
    return head_.compare_exchange_weak(from, to, mo);
  }

  State GetCurrent() { return head_.load(std::memory_order_acquire); }

  State Unlocked() { return nullptr; }

  bool IsUnlocked(State state) { return state == Unlocked(); }
  State SingleLocked() { return (IChainAwaiter *)1; }

  bool IsSingleLocked(State state) { return state == SingleLocked(); }

public:
  void lock() { // NOLINT
    Lock();
  }

  void unlock() { // NOLINT
    Unlock();
  }

private:
  // nullptr aka 0 -> Unlocked
  // (LockAwaiter*)1 aka 1 -> SingleLocked
  // LockAwaiter* -> Queue
  std::atomic<State> head_{nullptr};
};

} // namespace fib
