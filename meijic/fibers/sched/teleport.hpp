#pragma once
#include <meijic/executors/executor.hpp>
namespace fibers::sched {
void TeleportTo(executors::IExecutor *to);
}