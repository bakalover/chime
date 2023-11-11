#include <iostream>
#include <meijic/fibers/fiber.hpp>
namespace fib {

Fiber::Fiber(exe::IExecutor *sched, exe::ITask *routine)
    : core_(routine), sched_(sched) {}

void Fiber::Step() noexcept {
  me = this;
  core_.Resume();
  Reschedule();
}

void Fiber::Reschedule() {
  if (!core_.IsCompleted()) {
    sched_->Submit(this);
  } else {
    delete this;
  }
}

void Fiber::Yield() {
  if (me != nullptr) {
    me->core_.Suspend();
  }
}

void Fiber::Run() noexcept { Step(); }

Fiber *Fiber::Self() { return me; }

} // namespace fibers