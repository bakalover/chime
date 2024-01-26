#pragma once

#include <algorithm>
#include <chime/executors/task.hpp>
#include <chime/support/spinlock.hpp>
#include <cmath>
#include <cstddef>
#include <span>
#include <wheels/intrusive/forward_list.hpp>

namespace executors {

// Unbounded queue shared between workers

class GlobalQueue {
public:
  void Push(TaskBase *item) {
    support::SpinLock::Guard guard{spinlock_};
    queue_.PushBack(item);
  }

  void Offload(std::span<TaskBase *> buffer) {
    wheels::IntrusiveForwardList<TaskBase> tmp;
    for (size_t i = 0; i < buffer.size(); ++i) {
      tmp.PushBack(buffer[i]);
    }

    {
      support::SpinLock::Guard guard{spinlock_};
      queue_.Append(tmp); // Less workload on SpinLock
    }
  }

  // Returns nullptr if queue is empty
  TaskBase *TryPop() {
    support::SpinLock::Guard guard{spinlock_};
    return queue_.PopFront();
  }

  // Returns number of items in `out_buffer`
  size_t Grab(std::span<TaskBase *> out_buffer, size_t workers) {
    support::SpinLock::Guard guard{spinlock_};

    if (queue_.IsEmpty()) {
      return 0;
    }

    // Fairness
    size_t actual_part = std::max<size_t>(queue_.Size() / workers, 1);
    size_t actual_grab = std::min(out_buffer.size(), actual_part);

    for (size_t i = 0; i < actual_grab; ++i) {
      // Secure due to definition of actual_grab and Guard Lock
      out_buffer[i] = queue_.PopFront();
    }

    return actual_grab;
  }

private:
  wheels::IntrusiveForwardList<TaskBase> queue_;
  support::SpinLock spinlock_;
};

} // namespace executors
