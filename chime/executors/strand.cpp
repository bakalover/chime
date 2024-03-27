
#include <chime/executors/strand.hpp>
#include <chime/executors/tasks/hint.hpp>
#include <cstddef>

namespace executors {
namespace internal {

ActualStrand::ActualStrand(IExecutor &underlying) : underlying_{underlying} {}

void ActualStrand::Submit(TaskBase *task, SchedulerHint) {
  {
    support::SpinLock::Guard guard{spinlock_};
    queue_.PushBack(task);
  }
  if (count_.fetch_add(1) == 0) {
    SubmitSelf();
  }
}

void ActualStrand::Run() noexcept {
  const size_t done = RunTasks();
  const size_t left = count_.fetch_sub(done);
  if (left > done) {
    SubmitSelf();
  }
  this->ShortenLife();
}

size_t ActualStrand::RunTasks() {
  size_t done = 0;
  {
    support::SpinLock::Guard guard{spinlock_};
    while (queue_.NonEmpty()) {
      queue_.PopFront()->Run();
      ++done;
    }
  }
  return done;
}
void ActualStrand::SubmitSelf() {
  this->ExtendLife();
  underlying_.Submit(this, SchedulerHint::UpToYou);
}
} // namespace internal

Strand::Strand(IExecutor &underlying)
    : strand_(new internal::ActualStrand{underlying}) {}

void Strand::Submit(TaskBase *task, SchedulerHint) { strand_->Submit(task, SchedulerHint::UpToYou); }

} // namespace executors