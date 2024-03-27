#pragma once

#include <chime/executors/tasks/hint.hpp>
#include <chime/executors/tasks/executor.hpp>
#include <chime/fibers/fwd.hpp>

namespace fibers {

// Lightweight non-owning handle to a _suspended_ fiber
class FiberHandle {
  friend class Fiber;

public:
  FiberHandle() : FiberHandle(nullptr) {}

  static FiberHandle Invalid() { return FiberHandle(nullptr); }

  bool IsValid() const { return fiber_ != nullptr; }

  void Schedule(executors::SchedulerHint hint);

  // Switch to this fiber immediately
  void Switch();

  void ScheduleVia(executors::IExecutor *scheduler);

private:
  explicit FiberHandle(Fiber *fiber) : fiber_(fiber) {}

  Fiber *Release();

private:
  Fiber *fiber_;
};
} // namespace fibers
