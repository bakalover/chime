#pragma once
#include <meijic/fibers/awaiter.hpp>
namespace exe::fibers {

void Suspend(IAwaiter *awaiter);

} // namespace exe::fibers
