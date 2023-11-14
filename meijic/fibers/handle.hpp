#pragma once

#include <meijic/fibers/fwd.hpp>

namespace fib {

// Lightweight non-owning handle to a _suspended_ fiber
class FiberHandle {
  friend class Fiber;

public:
  FiberHandle() : FiberHandle(nullptr) {}

  static FiberHandle Invalid() { return FiberHandle(nullptr); }

  bool IsValid() const { return fiber_ != nullptr; }

  void Schedule();

  // Switch to this fiber immediately
  void Switch();

private:
  explicit FiberHandle(Fiber *fiber) : fiber_(fiber) {}

  Fiber *Release();

private:
  Fiber *fiber_;
};
} // namespace fib
