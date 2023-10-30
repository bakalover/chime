#pragma once
#include <meijic/executors/task.hpp>
namespace exec {

struct Executor {
  virtual ~Executor() = default;
  virtual void Submit(Task *task) = 0;
};

} // namespace exec
