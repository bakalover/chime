#include <meijic/executors/impl/just.hpp>
namespace exec {
class JustExecutor : public Executor {
  void Submit(Runnable *task) override { task->Run(); }
};

Executor &JustGet() {
  static JustExecutor instance;
  return instance;
}
} // namespace exec