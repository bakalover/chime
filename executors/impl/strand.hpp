#pragma once
#include "meijic/executors/task.hpp"
#include <atomic>
#include <meijic/executors/executor.hpp>
#include <meijic/support/locks/spinlock.hpp>
#include <queue>

namespace exec {
class Strand : public Executor, Task {
public:
  explicit Strand(Executor &underlying);

  // Non-copyable
  Strand(const Strand &) = delete;
  Strand &operator=(const Strand &) = delete;

  // Non-movable
  Strand(Strand &&) = delete;
  Strand &operator=(Strand &&) = delete;

  void Submit(Task *task) override;

private:
  void SubmitSelf();
  void Run() noexcept override;
  size_t RunTasks();

private:
  exec::Executor &underlying_;
  std::atomic<size_t> count_{0};
  // TODO: Transfer to MPSCQueue (Lock-free!!)
  std::queue<Task *> queue_;
  supp::SpinLock spinlock_;
};

} // namespace exec