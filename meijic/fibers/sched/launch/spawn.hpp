#pragma once
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fiber.hpp>
namespace fib::launch {
void Spawn(exe::IExecutor &scheduler, exe::TaskBase *task) {
  scheduler.Submit(new fib::Fiber(&scheduler, task));
};
} // namespace fib::launch