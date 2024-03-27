#include <chime/executors/scheduler/coordinator.hpp>
#include <chime/support/spinlock.hpp>
#include <cstdint>

namespace executors::scheduler {

bool Coordinator::TrySpin() {
  auto state = state_.load();
  if ((state & spin_mask_) * 2 >= threads_) {
    return false;
  }
  state_.fetch_add(1);
  return true;
}

bool Coordinator::StopSpin() {
  auto state = state_.fetch_sub(1);
  return (state & spin_mask_) == 1; // Last spinner
}

void Coordinator::BecomeIdle(Worker *worker) {
  support::SpinLock::Guard guard{spinlock_};
  idle_workers_.PushBack(worker);
  state_.fetch_add(idle_inc_);
}

void Coordinator::BecomeActive() { state_.fetch_sub(idle_inc_); }

bool Coordinator::IsAllAsleep() {
  auto state = state_.load();
  return ((state & idle_mask_) > 0) && ((state & spin_mask_) == 0);
}

void Coordinator::WakeIfIdle() {
  if (IsAllAsleep()) {
    WakeOne();
  }
}

void Coordinator::WakeOne() {
  support::SpinLock::Guard guard{spinlock_};
  if (Worker *worker = idle_workers_.PopFront()) {
    state_.fetch_sub(idle_inc_);
    worker->Wake(); // Maybe wake or not (cause not parked in 2-phase parking)
  }
}

} // namespace executors::scheduler
