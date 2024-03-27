#include "chime/executors/scheduler/coordinator.hpp"
#include "chime/executors/scheduler/worker.hpp"
#include "chime/executors/tasks/hint.hpp"
#include <algorithm>
#include <chime/executors/scheduler/scheduler.hpp>
#include <cstddef>

namespace executors::scheduler {

Scheduler::Scheduler(size_t threads)
    : threads_(threads), coordinator_{threads} {
  Start();
}

void Scheduler::Start() {
  for (size_t i = 0; i < threads_; ++i) {
    workers_.emplace_back(Worker{*this, i});
  }
  for (auto &worker : workers_) {
    worker.Start();
  }
}

Scheduler::~Scheduler() {
  WHEELS_ASSERT(workers_.empty(), "Scheduler still running!");
}

void Scheduler::Submit(TaskBase * /*task*/, SchedulerHint) {
  // Not implemented
}

void Scheduler::Stop() {
  
}

PoolMetrics Scheduler::Metrics() const {
  std::abort(); // Not yet implemented
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
