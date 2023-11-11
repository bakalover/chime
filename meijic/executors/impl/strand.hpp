#pragma once
#include <meijic/executors/task.hpp>
#include <atomic>
#include <meijic/executors/executor.hpp>
#include <meijic/support/locks/spinlock.hpp>
#include <queue>

namespace exe {
class Strand : public IExecutor, TaskBase {
public:
  explicit Strand(IExecutor &underlying);

  // Non-copyable
  Strand(const Strand &) = delete;
  Strand &operator=(const Strand &) = delete;

  // Non-movable
  Strand(Strand &&) = delete;
  Strand &operator=(Strand &&) = delete;

  void Submit(TaskBase *task) override;

private:
  void SubmitSelf();
  void Run() noexcept override;
  size_t RunTasks();

private:
  exe::IExecutor &underlying_;
  std::atomic<size_t> count_{0};
  // TODO: Transfer to MPSCQueue (Lock-free!!)
  std::queue<TaskBase*> queue_;
  sup::SpinLock spinlock_;
};

} // namespace exe