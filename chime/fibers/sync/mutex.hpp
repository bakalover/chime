#pragma once

#include <cassert>
#include <chime/fibers/awaiter.hpp>
#include <chime/fibers/fiber.hpp>
#include <chime/fibers/handle.hpp>
#include <cstddef>
#include <twist/ed/stdlike/atomic.hpp>

namespace fibers {

class Mutex {
private:
  struct LockAwaiter : ChainAwaiterBase {
    explicit LockAwaiter(Mutex &host) : host_(host) {}

    void AwaitSuspend(FiberHandle handle) override {
      handle_ = handle;
      if (host_.Acquire(this)) {
        handle_.Schedule();
      };
    }

    Mutex &host_;
  };

  using State = ChainAwaiterBase *;

public:
  void Lock() {
    LockAwaiter lock_awaiter{*this};
    if (Fiber::InContext()) {
      Fiber::Self()->Suspend(&lock_awaiter);
    }
  }

  void Unlock() {
    while (true) {
      // Prerequisite: head_ != Unlocked =>
      // => head_ \in {SingleLocked; Queue}
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

  State SingleLocked() { return (ChainAwaiterBase *)1; }

  bool IsUnlocked(State state) { return state == Unlocked(); }

  bool IsSingleLocked(State state) { return state == SingleLocked(); }

public:
  void lock() { Lock(); }

  void unlock() { Unlock(); }

private:
  // nullptr aka 0 -> Unlocked
  // (IChainAwaiter*)1 aka 1 -> SingleLocked
  // IChainAwaiter* -> Queue
  twist::ed::stdlike::atomic<State> head_{nullptr};
};

} // namespace fibers
