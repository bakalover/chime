#include <cassert>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/handle.hpp>
#include <meijic/fibers/routine.hpp>
#include <utility>

namespace fibers {

Fiber::Fiber(executors::IExecutor *sched, IRoutine *routine)
    : coro_{routine}, sched_{sched} {}

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
  CURRENT_FIBER = this;
  coro_.Resume();
}

void Fiber::ActivateAwaiter() {
  auto awaiter = std::exchange(awaiter_, nullptr);
  assert(awaiter != nullptr);
  awaiter->AwaitSuspend(FiberHandle{this});
}

void Fiber::Switch() { Run(); }

void Fiber::Run() noexcept {
  Step();
  if (coro_.IsCompleted()) {
    EndLifeCycle();
    return; // delete???
  }
  ActivateAwaiter();
}

void Fiber::EndLifeCycle() {
  // Structured concurrency
  // manipulator_->Manipulate(FiberHandle{this})
  SelfDestroy(); // delete???
}

void Fiber::SelfDestroy() { delete this; }

Fiber *Fiber::Self() { return CURRENT_FIBER; }

bool Fiber::InContext() { return Fiber::Self() != nullptr; }

} // namespace fibers
