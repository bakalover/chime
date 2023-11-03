#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <queue>
namespace exec {

class ManualExecutor : public IExecutor {
public:
  ManualExecutor() = default;

  // Non-copyable
  ManualExecutor(const ManualExecutor &) = delete;
  ManualExecutor &operator=(const ManualExecutor &) = delete;

  // Non-movable
  ManualExecutor(ManualExecutor &&) = delete;
  ManualExecutor &operator=(ManualExecutor &&) = delete;

  void Submit(TaskBase *task) override;

  size_t RunAtMost(size_t limit);

  bool RunNext() { return RunAtMost(1) == 1; }

  size_t Drain();

  size_t TaskCount() const { return queue_.size(); }

  bool IsEmpty() const { return queue_.empty(); }

  bool NonEmpty() const { return !IsEmpty(); }

private:
  std::queue<TaskBase *> queue_;
};

} // namespace exec
