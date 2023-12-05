#pragma once
#include <meijic/executors/task.hpp>
#include <meijic/fibers/fiber.hpp>
namespace fibers::sched::launch {
void Spawn(executors::IExecutor &scheduler, executors::TaskBase *task) {
  scheduler.Submit(new fibers::Fiber(&scheduler, task));
};
} // namespace fibers::sched::launch