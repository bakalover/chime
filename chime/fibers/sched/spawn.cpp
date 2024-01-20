#include <chime/executors/container.hpp>
#include <chime/executors/executor.hpp>
#include <chime/executors/task.hpp>
#include <chime/fibers/fwd.hpp>
#include <chime/fibers/routine.hpp>
#include <chime/fibers/sched/spawn.hpp>
#include <utility>

namespace fibers {

namespace internal {
void Spawn(executors::IExecutor *scheduler, IRoutine *task) {
  // + manipulator field
  scheduler->Submit(new Fiber(scheduler, task));
}

} // namespace internal
} // namespace fibers::sched