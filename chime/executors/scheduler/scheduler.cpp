#include "chime/executors/scheduler/coordinator.hpp"
#include "chime/executors/scheduler/worker.hpp"
#include <chime/executors/scheduler/scheduler.hpp>

namespace executors::scheduler {

Scheduler::Scheduler(size_t threads)
    : threads_{threads}, coordinator_{threads} {
  // Not implemented
}

void Scheduler::Start() {
  // Not implemented
}

Scheduler::~Scheduler() {
  // Not implemented
}

void Scheduler::Submit(TaskBase * /*task*/) {
  // Not implemented
}

void Scheduler::Stop() {
  // Not implemented
}

PoolMetrics Scheduler::Metrics() const {
  std::abort(); // Not implemented
}

Scheduler *Scheduler::Current() {
  Worker *worker = Worker::Current();
  if (worker == nullptr) {
    return nullptr;
  } else {
    return &worker->Host();
  }
}

} // namespace executors::scheduler
