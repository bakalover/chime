#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/fibers/api/spawner.hpp>
namespace fibers {
 Spawner *Via(exec::IExecutor &scheduler);
} // namespace fibers