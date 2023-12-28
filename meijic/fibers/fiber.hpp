#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/coro/coro.hpp>

namespace fibers {

// Fiber = stackful coroutine + executor::IExecutor (executor)

class Fiber : public executors::TaskBase {
public:
  Fiber(executors::IExecutor *sched, TaskBase *routine);

  void SetScheduler(executors::IExecutor *scheduler);

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
  executors::IExecutor *sched_;
  IAwaiter *awaiter_;
};
static thread_local Fiber *me = nullptr;
} // namespace fibers
