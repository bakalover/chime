#include <chime/executors/manual.hpp>

namespace executors {
void ManualExecutor::Submit(TaskBase *task) { queue_.PushBack(task); }

size_t ManualExecutor::RunAtMost(size_t limit) {
  size_t task_number = std::min(limit, queue_.Size());
  for (size_t i = 0; i < task_number; ++i) {
    queue_.PopFront()->Run();
  }
  return task_number;
}

size_t ManualExecutor::Drain() {
  size_t done = 0;
  while (queue_.NonEmpty()) {
    queue_.PopFront()->Run();
    ++done;
  }
  return done;
}
} // namespace executors
