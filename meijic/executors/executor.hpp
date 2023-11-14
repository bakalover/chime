#pragma once
#include <meijic/executors/task.hpp>
namespace exe {
struct IExecutor {
  virtual ~IExecutor() = default;
  virtual void Submit(TaskBase *task) = 0;
};

} // namespace exe
