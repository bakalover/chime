#pragma once

#include <chime/executors/tasks/hint.hpp>
#include <chime/executors/tasks/task.hpp>
#include <chime/fibers/awaiter.hpp>
#include <chime/fibers/fiber.hpp>
#include <chime/fibers/handle.hpp>
#include <chime/fibers/sync/await_list.hpp>
#include <chime/support/spinlock.hpp>

namespace fibers {

// One-shot

class Event {
private:
  struct Waiter : ChainAwaiterBase {
    explicit Waiter(Event &event) : event_(event) {}

    void AwaitSuspend(FiberHandle handle) override {
      // Prepare for sequential wake up in Fire()
      handle_ = handle;
      // Add me to wait queue or schedule if queue is sealed
      if (event_.Wait(this)) {
        return;
      } else {
        handle_.Schedule(executors::SchedulerHint::Next);
      }
      // Reordering 2 rows above -> datarace on handle in Fire/Wait functions
      // interleaving
    }
    Event &event_;
  };

public:
  void Wait() {
    if (Fiber::InContext()) {
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

  void ChainResume(ChainAwaiterBase *awaiter) {
    ChainAwaiterBase *current = awaiter;
    while (current != nullptr) {
      ChainAwaiterBase *next = current->next_;
      WakeUpFiber(current->handle_);
      current = next;
    }
  }

  void WakeUpFiber(FiberHandle handle) {
    handle.Schedule(executors::SchedulerHint::UpToYou);
  }

private:
  internal::AwaitList wait_queue_;
  support::SpinLock spinlock_;
};

} // namespace fibers
