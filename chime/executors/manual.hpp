#pragma once
#include <wheels/intrusive/list.hpp>
#include <chime/executors/executor.hpp>
#include <chime/executors/task.hpp>
#include <queue>

namespace executors {

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

  size_t TaskCount() const { return queue_.Size(); }

  bool IsEmpty() const { return queue_.IsEmpty(); }

  bool NonEmpty() const { return !IsEmpty(); }

private:
  wheels::IntrusiveForwardList<TaskBase> queue_;
};

} // namespace executors
