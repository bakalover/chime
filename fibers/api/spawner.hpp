#pragma once
#include "meijic/executors/container.hpp"
#include "meijic/executors/executor.hpp"
#include "meijic/fibers/fiber.hpp"
#include <utility>
namespace fibers {
struct Spawner {

  explicit Spawner(exec::IExecutor &scheduler) : scheduler_(scheduler){};

  template <typename Fun> Spawner *Spawn(Fun fun) {
    scheduler_.Submit(
        new Fiber(scheduler_, exec::MakeContainer(std::move(fun))));
    return this;
  }

  void Via(exec::IExecutor &scheduler) { scheduler_ = scheduler; };

  void Enough() { delete this; }

private:
  // TODO: Stack configuration
  exec::IExecutor &scheduler_;
};
} // namespace fibers