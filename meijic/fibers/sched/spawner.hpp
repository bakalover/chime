#pragma once
#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fiber.hpp>
#include <queue>
#include <utility>
namespace fib {
class Spawner {
public:
  explicit Spawner(exe::IExecutor &scheduler)
      : scheduler_(&scheduler), tasks_(){};

  template <typename Fun> Spawner *Spawn(Fun fun) {
    AddTask(exe::MakeContainer(std::move(fun)));
    return this;
  }

  Spawner *Via(exe::IExecutor &scheduler) {
    scheduler_ = &scheduler;
    return this;
  };

  void RunAll() {
    while (!tasks_.empty()) {
      RunSingle();
    }
    SelfDestroy();
  }

private:
  void AddTask(exe::TaskBase *task) { tasks_.push({scheduler_, task}); }
  void RunSingle() {
    auto [scheduler, task] = tasks_.front();
    scheduler->Submit(new Fiber(scheduler, task));
    tasks_.pop();
  }

  void SelfDestroy() { delete this; }

private:
  // TODO: Stack configuration
  exe::IExecutor *scheduler_;
  std::queue<std::pair<exe::IExecutor *, exe::TaskBase *>> tasks_;
};
} // namespace fibers