#pragma once

#include <chime/executors/tasks/hint.hpp>
#include <chime/executors/tasks/executor.hpp>
#include <chime/executors/tasks/task.hpp>
#include <chime/fibers/awaiter.hpp>
#include <chime/fibers/coro/coro.hpp>
#include <chime/fibers/routine.hpp>
#include <twist/ed/local/ptr.hpp>

namespace fibers {

TWISTED_THREAD_LOCAL_PTR_INIT(Fiber, CURRENT_FIBER, nullptr)

// Fiber = stackful coroutine + executor::IExecutor (executor)

class Fiber : public executors::TaskBase {
public:
  // + Manipulator field
  Fiber(executors::IExecutor *sched, IRoutine *routine);

  void SetScheduler(executors::IExecutor *scheduler);

  executors::IExecutor *GetScheduler();

  void Suspend(IAwaiter *awaiter);

  void Schedule(executors::SchedulerHint hint);

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
  Coroutine coro_;
  executors::IExecutor *sched_;
  IAwaiter *awaiter_;
};

} // namespace fibers
