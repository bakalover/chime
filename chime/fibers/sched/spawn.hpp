#pragma once

#include <chime/executors/tasks/executor.hpp>
#include <chime/executors/tasks/task.hpp>
#include <chime/fibers/fiber.hpp>
#include <chime/fibers/fwd.hpp>
#include <chime/fibers/routine.hpp>

namespace fibers {

namespace internal {
void Spawn(executors::IExecutor *scheduler, IRoutine *task);
}

template <typename L> void Spawn(executors::IExecutor &scheduler, L &&lambda) {
  internal::Spawn(&scheduler, MakeRoutine(std::move(lambda)));
};

template <typename L> void Spawn(L &&lambda) {
  if (Fiber::InContext()) {
    internal::Spawn(Fiber::Self()->GetScheduler(),
                    MakeRoutine(std::move(lambda)));
  }
};

} // namespace fibers