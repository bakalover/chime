#pragma once

#include <chime/executors/task.hpp>
#include <chime/fibers/fwd.hpp>
#include <chime/executors/container.hpp>
#include <chime/executors/executor.hpp>
#include <chime/fibers/fiber.hpp>

namespace fibers::sched {
template <typename L> void Spawn(executors::IExecutor &scheduler, L &&lambda);

void Spawn(executors::IExecutor *scheduler, executors::TaskBase *task);

// TODO some kind of context to store cuurent executor -> Spawn(Task*)

} // namespace fibers::sched