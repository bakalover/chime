#pragma once
#include <meijic/executors/run.hpp>
namespace exec {

struct Executor {
  virtual ~Executor() = default;
  virtual void Submit(Runnable *task) = 0;
};

} // namespace exec
