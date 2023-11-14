#pragma once
#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/fibers/sched/launch/spawn.hpp>
namespace fib::sched {
template <typename L> void SpawnVia(exe::IExecutor &scheduler, L lambda) {
  launch::Spawn(scheduler, new exe::MakeContainer(std::move(lambda)));
};

// TODO some kind of context to store cuurent executor -> Spawn(Task*)

} // namespace fib::sched