#include <meijic/executors/impl/just.hpp>
namespace exec {
class JustExecutor : public Executor {
  void Submit(Task *task) override { task->Run(); }
};

Executor &JustGet() {
  static JustExecutor instance;
  return instance;
}
} // namespace exec