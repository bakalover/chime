#pragma once
#include <meijic/fibers/awaiter.hpp>
namespace fibers::sched {

void Suspend(IAwaiter *awaiter);

} // namespace fib
