#pragma once
#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/fibers/sched/launch/spawn.hpp>

namespace fibers::sched {
template <typename L>
void SpawnVia(executors::IExecutor &scheduler, L &&lambda) {
  launch::Spawn(scheduler, executors::MakeContainer(std::move(lambda)));
};

// TODO some kind of context to store cuurent executor -> Spawn(Task*)

} // namespace fibers::sched