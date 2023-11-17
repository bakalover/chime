#pragma once

// std::lock_guard and std::unique_lock
#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/handle.hpp>
#include <mutex>
#include <ostream>

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
      LockAwaiter *current_head = head_.load(std::memory_order_acquire);

      assert(current_head != nullptr);

      if (current_head == (LockAwaiter *)1) {
        if (head_.compare_exchange_weak(current_head, nullptr)) {
          return;
        }
        continue;
      } else {
        if (head_.compare_exchange_weak(current_head, current_head->next_)) {
          current_head->handle_.Schedule();
          return;
        }
        continue;
      }
    }
  }

  bool Acquire(LockAwaiter *awaiter) {
    while (true) {
      LockAwaiter *cur_head = head_.load();
      if (cur_head == nullptr) {
        if (head_.compare_exchange_weak(cur_head, (LockAwaiter *)1)) {
          return true;
        }
        continue;
      } else {
        awaiter->next_ = cur_head;
        if (head_.compare_exchange_weak(cur_head, awaiter)) {
          return false;
        }
        continue;
      }
    }
  }

  // BasicLockable

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
  std::atomic<LockAwaiter *> head_{nullptr};
};

} // namespace fib
