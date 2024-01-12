#pragma once

#include <meijic/executors/task.hpp>
#include <meijic/fibers/fwd.hpp>
#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/fibers/fiber.hpp>

namespace fibers::sched {
template <typename L> void Spawn(executors::IExecutor &scheduler, L &&lambda);

void Spawn(executors::IExecutor *scheduler, executors::TaskBase *task);

// TODO some kind of context to store cuurent executor -> Spawn(Task*)

} // namespace fibers::sched