#include <cassert>
#include <chime/fibers/fiber.hpp>
#include <chime/fibers/handle.hpp>
#include <utility>

namespace fibers {

Fiber *FiberHandle::Release() {
  assert(IsValid());
  return std::exchange(fiber_, nullptr);
}

void FiberHandle::Schedule() { Release()->Schedule(); }

void FiberHandle::Switch() { Release()->Switch(); }

void FiberHandle::ScheduleVia(executors::IExecutor *scheduler) {
  assert(scheduler != nullptr);
  fiber_->SetScheduler(scheduler);
  Schedule();
}

} // namespace fibers
