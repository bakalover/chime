#pragma once
#include "twist/rt/facade/static/thread_local/ptr.hpp"
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/coro/coro.hpp>
#include <twist/ed/static/thread_local/ptr.hpp>

namespace fibers {

TWISTED_STATIC_THREAD_LOCAL_PTR(Fiber, me);

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

} // namespace fibers
