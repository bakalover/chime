#include <chime/executors/impl/inline.hpp>
namespace executors {

class InlineExecutor : public IExecutor {
public:
  // IExecutor
  void Submit(TaskBase *task) override { task->Run(); }
};

IExecutor &Inline() {
  static InlineExecutor instance;
  return instance;
}
} // namespace executors