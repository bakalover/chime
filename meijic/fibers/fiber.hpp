#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/coro/coro.hpp>

namespace fib {

// Fiber = stackful coroutine + exe::IExecutor (executor)

class Fiber : exe::TaskBase {
public:
  Fiber(exe::IExecutor *sched, TaskBase *routine);
  void Suspend(IAwaiter *awaiter);

  void Schedule();
  void Switch();
  void Await();
  // Task
  void Run() noexcept override;

  static Fiber *Self();

  static bool IsFiber();

private:
  void Step() noexcept;

private:
  coro::Coroutine coro_;
  exe::IExecutor *sched_;
  IAwaiter *awaiter_;
};
static thread_local Fiber *me = nullptr;
} // namespace fib
