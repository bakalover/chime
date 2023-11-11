#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/fibers/sched/spawner.hpp>
namespace fib {
Spawner *Via(exe::IExecutor &scheduler);
} // namespace fibers