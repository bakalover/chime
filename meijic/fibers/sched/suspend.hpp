#pragma once
#include <meijic/fibers/awaiter.hpp>
namespace fib {

void Suspend(IAwaiter *awaiter);

} // namespace fib
