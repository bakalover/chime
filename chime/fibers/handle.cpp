#include <chime/executors/tasks/hint.hpp>
#include <cassert>
#include <chime/fibers/fiber.hpp>
#include <chime/fibers/handle.hpp>
#include <utility>

namespace fibers {

Fiber *FiberHandle::Release() {
  assert(IsValid());
  return std::exchange(fiber_, nullptr);
}

void FiberHandle::Schedule(executors::SchedulerHint hint) {
  Release()->Schedule(hint);
}

void FiberHandle::Switch() { Release()->Switch(); }

void FiberHandle::ScheduleVia(executors::IExecutor *scheduler) {
  assert(scheduler != nullptr);
  fiber_->SetScheduler(scheduler);
  Schedule(executors::SchedulerHint::UpToYou);
}

} // namespace fibers
