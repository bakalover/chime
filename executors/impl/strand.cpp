
#include <cstddef>
#include <meijic/executors/impl/strand.hpp>
namespace exec {

Strand::Strand(Executor &underlying)
    : underlying_(underlying), queue_(), spinlock_() {}

void Strand::Submit(Runnable *task) {
  {
    supp::SpinLock::Guard guard(spinlock_);
    queue_.push(std::move(task));
  }
  if (count_.fetch_add(1) == 0) {
    SubmitSelf();
  }
}

void Strand::Run() noexcept {
  const size_t done = RunTasks();
  const size_t left = count_.fetch_sub(done);
  if (left > done) {
    SubmitSelf();
  }
}

size_t Strand::RunTasks() {
  size_t done = 0;
  {
    supp::SpinLock::Guard guard(spinlock_);
    while (!queue_.empty()) {
      queue_.front()->Run();
      queue_.pop();
      ++done;
    }
  }
  return done;
}
// TODO: Lifetime problems
void Strand::SubmitSelf() { underlying_.Submit(this); }
} // namespace exec