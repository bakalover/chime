#pragma once

#include <chime/executors/executor.hpp>
#include <chime/executors/task.hpp>
#include <chime/fibers/fiber.hpp>
#include <chime/fibers/fwd.hpp>
#include <chime/fibers/routine.hpp>

namespace fibers::sched {

namespace internal {
void Spawn(executors::IExecutor *scheduler, IRoutine *task);
}

template <typename L> void Spawn(executors::IExecutor &scheduler, L &&lambda) {
  internal::Spawn(&scheduler, MakeRoutine(std::move(lambda)));
};

// TODO some kind of context to store cuurent executor -> Spawn(IRoutine*)

} // namespace fibers::sched