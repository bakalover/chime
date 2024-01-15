#pragma once
#include <atomic>
#include <chime/executors/executor.hpp>
#include <chime/executors/task.hpp>
#include <chime/support/life/management.hpp>
#include <chime/support/spinlock.hpp>
#include <twist/ed/stdlike/atomic.hpp>
#include <wheels/intrusive/forward_list.hpp>

namespace executors {
namespace internal {
class ActualStrand : private IExecutor,
                     private TaskBase,
                     public support::LifeManagableBase<ActualStrand> {
public:
  explicit ActualStrand(IExecutor &underlying);

  // Non-copyable
  ActualStrand(const ActualStrand &) = delete;
  ActualStrand &operator=(const ActualStrand &) = delete;

  // Non-movable
  ActualStrand(ActualStrand &&) = delete;
  ActualStrand &operator=(ActualStrand &&) = delete;

  void Submit(TaskBase *task) override;

private:
  void SubmitSelf();
  void Run() noexcept override;
  size_t RunTasks();

private:
  executors::IExecutor &underlying_;
  twist::ed::stdlike::atomic<size_t> count_{0};
  // TODO: Transfer to MPSCQueue (Lock-free!!)
  wheels::IntrusiveForwardList<TaskBase> queue_;
  support::SpinLock spinlock_;
};
} // namespace internal

class Strand : public IExecutor {
public:
  explicit Strand(IExecutor &underlying);

  ~Strand() { strand_->ShortenLife(); }

  // Non-copyable
  Strand(const Strand &) = delete;
  Strand &operator=(const Strand &) = delete;

  // Non-movable
  Strand(Strand &&) = delete;
  Strand &operator=(Strand &&) = delete;

  void Submit(TaskBase *task) override;

private:
  internal::ActualStrand *strand_;
};

} // namespace executors