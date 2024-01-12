#pragma once

#include <twist/rt/facade/static/thread_local/ptr.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/coro/coro.hpp>
#include <twist/ed/static/thread_local/ptr.hpp>
#include <meijic/fibers/routine.hpp>

namespace fibers {

TWISTED_STATIC_THREAD_LOCAL_PTR(Fiber, CURRENT_FIBER);

// Fiber = stackful coroutine + executor::IExecutor (executor)

class Fiber : public executors::TaskBase {
public:
  // + Manipulator field
  Fiber(executors::IExecutor *sched, IRoutine* routine);

  void SetScheduler(executors::IExecutor *scheduler);

  void Suspend(IAwaiter *awaiter);

  void Schedule();

  void Switch();

  void ActivateAwaiter();

  // Task
  void Run() noexcept override;

  static Fiber *Self();

  static bool InContext();

private:
  void Step() noexcept;

  void SelfDestroy();

  void EndLifeCycle();

private:
  // manupulator_
  coro::Coroutine coro_;
  executors::IExecutor *sched_;
  IAwaiter *awaiter_;
};

} // namespace fibers
