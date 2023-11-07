#pragma once
#include <meijic/executors/task.hpp>
namespace exec {

struct IExecutor {
  virtual ~IExecutor() = default;
  virtual void Submit(TaskBase *task) = 0;
};

} // namespace exec
