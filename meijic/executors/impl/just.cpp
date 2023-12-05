#include <meijic/executors/impl/just.hpp>

namespace executors {
class JustExecutor : public IExecutor {
  void Submit(TaskBase *task) override { task->Run(); }
};

IExecutor &JustGet() {
  static JustExecutor instance;
  return instance;
}
} // namespace executors