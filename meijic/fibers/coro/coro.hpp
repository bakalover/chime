#pragma once

#include <meijic/executors/task.hpp>
#include <meijic/fibers/routine.hpp>
#include <sure/context.hpp>
#include <sure/stack.hpp>
#include <sure/trampoline.hpp>

namespace fibers::coro {
class Coroutine : private sure::ITrampoline {
public:
  explicit Coroutine(IRoutine *routine);

  void Resume() noexcept;

  void Suspend();

  bool IsCompleted() const { return is_complete_; }

private:
  // TODO: Get rid of stack
  sure::Stack stack_;
  IRoutine *routine_;
  sure::ExecutionContext me_, caller_;
  bool is_complete_ = false;

private:
  [[noreturn]] void Run() noexcept override;
  [[noreturn]] void Complete();
};
} // namespace fibers::coro
