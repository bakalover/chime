#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/coro.hpp>

namespace fib {
class Fiber : public exe::TaskBase {
public:
  // TODO: Resourse management
  Fiber(exe::IExecutor *sched, exe::ITask *routine);
  void Reschedule();

  void Yield();

  void Step() noexcept;

  static Fiber *Self();

  void Run() noexcept override;

private:
  coro::Coroutine core_;
  exe::IExecutor *sched_;
};

thread_local static Fiber *me = nullptr;
} // namespace fibers
