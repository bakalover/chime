#include <cassert>
#include <iostream>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/handle.hpp>
#include <ostream>
#include <utility>

namespace fibers {

Fiber::Fiber(executors::IExecutor *sched, executors::TaskBase *routine)
    : coro_(routine), sched_(sched) {}

void Fiber::SetScheduler(executors::IExecutor *scheduler) {
  sched_ = scheduler;
}

void Fiber::Suspend(IAwaiter *awaiter) {
  awaiter_ = awaiter;
  assert(awaiter != nullptr);
  coro_.Suspend();
}

void Fiber::Schedule() { sched_->Submit(this); }

void Fiber::Step() noexcept {
  me = this;
  coro_.Resume();
}

void Fiber::Await() {
  auto awaiter = std::exchange(awaiter_, nullptr);
  assert(awaiter != nullptr);
  awaiter->AwaitSuspend(FiberHandle{this});
}

void Fiber::Switch() { Run(); }

void Fiber::Run() noexcept {
  Step();
  if (coro_.IsCompleted()) {
    delete this;
    return;
  }
  Await();
}

Fiber *Fiber::Self() { return me; }

bool Fiber::IsFiber() { return Fiber::Self() != nullptr; }

} // namespace fibers
