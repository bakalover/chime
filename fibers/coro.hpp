#pragma once
#include <meijic/executors/run.hpp>
#include <meijic/fibers/context/context.hpp>
namespace fibers::coro
{
class Coroutine : private exec::Runnable{
  explicit Coroutine(exec::Runnable *routine);

  void Resume() noexcept;

  void Suspend();

  bool IsCompleted() const{
    return is_complete_;
  }

private:
  exec::Runnable *routine_;
  Context callee_, caller_;
  bool is_complete_ = false;

private:
  [[noreturn]] void Run() noexcept override;
  [[noreturn]] void Complete();
};
} // namespace fibers::coro

