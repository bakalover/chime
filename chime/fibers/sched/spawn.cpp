#include "chime/executors/tasks/hint.hpp"
#include <chime/executors/tasks/container.hpp>
#include <chime/executors/tasks/executor.hpp>
#include <chime/executors/tasks/task.hpp>
#include <chime/fibers/fwd.hpp>
#include <chime/fibers/routine.hpp>
#include <chime/fibers/sched/spawn.hpp>
#include <utility>

namespace fibers {

namespace internal {
void Spawn(executors::IExecutor *scheduler, IRoutine *task) {
  // + manipulator field
  scheduler->Submit(new Fiber(scheduler, task), executors::SchedulerHint::UpToYou);
}

} // namespace internal
} // namespace fibers::sched