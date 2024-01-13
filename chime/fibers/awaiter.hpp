#pragma once

#include <chime/fibers/handle.hpp>

namespace fibers {

struct IAwaiter {
  virtual void AwaitSuspend(FiberHandle handle) = 0;
};

struct IChainAwaiter : IAwaiter {

  // Link
  IChainAwaiter *next_;

  // Chain awaiter has to store it's handle until time to go
  FiberHandle handle_;
};

} // namespace fibers
