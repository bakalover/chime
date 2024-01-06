#pragma once
#include <atomic>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/handle.hpp>
#include <meijic/support/spinlock.hpp>
#include <meijic/support/queues/await.hpp>

namespace fibers::sync {

// One-shot

class Event {
private:
  struct Waiter : IChainAwaiter {
    explicit Waiter(Event &event) : event_(event) {}

    void AwaitSuspend(FiberHandle handle) override {
      // Prepare for sequential wake up in Fire()
      handle_ = handle;
      // Add me to wait queue or schedule if queue is sealed
      if (event_.Wait(this)) {
        return;
      } else {
        handle_.Schedule();
      }
      // Reordering 2 rows above -> datarace on handle in Fire/Wait functions
      // interleaving
    }
    Event &event_;
  };

public:
  void Wait() {
    if (Fiber::IsFiber()) {
      DoWait();
    }
  }

  void Fire() { ChainResume(wait_queue_.Close()); }

private:
  void DoWait() {
    if (wait_queue_.IsClosed()) {
      return;
    }
    Waiter waiter{*this};
    Fiber::Self()->Suspend(&waiter);
  }

  bool Wait(Waiter *waiter) { return wait_queue_.TryAdd(waiter); }

  void ChainResume(IChainAwaiter *awaiter) {
    IChainAwaiter *current = awaiter;
    while (current != nullptr) {
      IChainAwaiter *next = current->next_;
      WakeUpFiber(current->handle_);
      current = next;
    }
  }

  void WakeUpFiber(FiberHandle handle) { handle.Schedule(); }

private:
  support::queue::AwaitersList wait_queue_;
  support::SpinLock spinlock_;
};

} // namespace fibers::sync
