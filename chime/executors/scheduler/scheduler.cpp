#include <algorithm>
#include <chime/executors/scheduler/scheduler.hpp>
#include <chime/executors/scheduler/worker.hpp>
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

void Scheduler::Submit(TaskBase *task, SchedulerHint hint) {
  PushWithStrategy(task, hint);
  coordinator_.WakeOneIfIdle();
}

void Scheduler::PushWithStrategy(TaskBase *task, SchedulerHint hint) {
  if (Worker::InContext(this)) {
    Worker::Current()->PushWithStrategy(task, hint);
  } else {
    wg_.Add(); // Track Global task
    global_tasks_.Push(task);
  }
}

void Scheduler::Stop() {
  coordinator_.ShutDown();
  for (auto &worker : workers_) {
    worker.Join();
  }
}

void Scheduler::WaitIdle() { wg_.Wait(); }

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
