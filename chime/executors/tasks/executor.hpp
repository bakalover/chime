#pragma once
#include "chime/executors/tasks/hint.hpp"
#include <chime/executors/tasks/task.hpp>

namespace executors {
struct IExecutor {
  virtual ~IExecutor() = default;
  virtual void Submit(TaskBase *task, SchedulerHint hint) = 0;
};

} // namespace executors
