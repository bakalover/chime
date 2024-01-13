#pragma once

#include <chime/executors/executor.hpp>

namespace fibers::sched {
void TeleportTo(executors::IExecutor *to);
}