#pragma once

#include <meijic/fibers/handle.hpp>

namespace fib {

struct IAwaiter {
  virtual void AwaitSuspend(FiberHandle handle) = 0;
};

struct IChainAwaiter : IAwaiter {
  IChainAwaiter *next_;
  FiberHandle handle_;
};

} // namespace fib
