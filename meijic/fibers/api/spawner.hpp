#pragma once
#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fiber.hpp>
#include <queue>
#include <utility>
namespace fibers {
class Spawner {
public:
  explicit Spawner(exec::IExecutor &scheduler)
      : scheduler_(scheduler), tasks_(){};

  template <typename Fun> Spawner *Spawn(Fun fun) {
    AddTask(exec::MakeContainer(std::move(fun)));
    return this;
  }

  Spawner *Via(exec::IExecutor &scheduler) {
    scheduler_ = scheduler;
    return this;
  };

private:
  void AddTask(exec::TaskBase *task) { tasks_.push(task); }

  void RunAll() {
    while (!tasks_.empty()) {
      RunSingle();
    }
    SelfDestroy();
  }
  void RunSingle() {
    scheduler_.Submit(tasks_.front());
    tasks_.pop();
  }

  void SelfDestroy() { delete this; }

private:
  // TODO: Stack configuration
  exec::IExecutor &scheduler_;
  std::queue<exec::TaskBase *> tasks_;
};
} // namespace fibers