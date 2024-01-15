#pragma once

#include <chime/executors/executor.hpp>
#include <chime/executors/task.hpp>
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
