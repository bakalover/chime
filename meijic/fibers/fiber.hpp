#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/coro.hpp>

namespace fibers {
class Fiber : public exec::TaskBase {
public:
  // TODO: Resourse management
  Fiber(exec::IExecutor &sched, exec::ITask *routine);
  void Reschedule();

  void Yield();

  void Step() noexcept;

  static Fiber *Self();

  void Run() noexcept override;

private:
  coro::Coroutine core_;
  exec::IExecutor &sched_;
};

thread_local static Fiber *me = nullptr;
} // namespace fibers
