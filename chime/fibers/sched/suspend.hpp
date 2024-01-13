#pragma once

#include <chime/fibers/awaiter.hpp>

namespace fibers::sched {

void Suspend(IAwaiter *awaiter);

} // namespace fib
