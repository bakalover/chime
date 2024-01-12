#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fwd.hpp>
#include <meijic/fibers/routine.hpp>
#include <meijic/fibers/sched/spawn.hpp>
#include <utility>

namespace fibers::sched {
template <typename L> void Spawn(executors::IExecutor &scheduler, L &&labmda) {
  Spawn(&scheduler, executors::MakeContainer(std::move(labmda)));
}

void Spawn(executors::IExecutor *scheduler, IRoutine *task) {
  // + manipulator field
  scheduler->Submit(new Fiber(scheduler, task));
}
} // namespace fibers::sched