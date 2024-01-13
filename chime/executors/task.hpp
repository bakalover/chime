#pragma once
#include <wheels/intrusive/forward_list.hpp>

namespace executors {
struct ITask {
  virtual ~ITask() = default;

  virtual void Run() noexcept = 0;
};

struct TaskBase : ITask, wheels::IntrusiveForwardListNode<TaskBase> {};
} // namespace executors
