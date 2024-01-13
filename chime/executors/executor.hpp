#pragma once
#include <chime/executors/task.hpp>

namespace executors {
struct IExecutor {
  virtual ~IExecutor() = default;
  virtual void Submit(TaskBase *task) = 0;
};

} // namespace executors
