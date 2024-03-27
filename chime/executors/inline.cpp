
#include <chime/executors/inline.hpp>
namespace executors {

class InlineExecutor : public IExecutor {
public:
  // IExecutor
  void Submit(TaskBase *task, SchedulerHint) override { task->Run(); }
};

IExecutor &Inline() {
  static InlineExecutor instance;
  return instance;
}
} // namespace executors