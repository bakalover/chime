#include <algorithm>
#include <chime/executors/scheduler/scheduler.hpp>
#include <chime/executors/scheduler/worker.hpp>
#include <cstddef>

namespace executors::scheduler {

Scheduler::Scheduler(size_t threads)
    : threads_{threads}, coordinator_{threads} {
    Start();
}

void Scheduler::Start() {
  for (size_t i = 0; i < threads_; ++i) {
    workers_.emplace_back(*this, i);
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
  if (Worker::InContextOf(this)) {
    Worker::Current()->PushWithStrategy(task, hint);
  } else {
    activity_.Add(); // Track Global task
    global_tasks_.Push(task);
  }
}

void Scheduler::Stop() {
  coordinator_.ShutDown();
  for (auto &worker : workers_) {
    worker.Join();
  }
  workers_.clear();
}

void Scheduler::WaitIdle() { activity_.Wait(); }

PoolMetrics Scheduler::Metrics() const {
  std::abort(); // Not yet implemented
}

Scheduler *Scheduler::Current() {

  if (Worker::InContext()) {
    return &Worker::Current()->Host();
  } else {
    return nullptr;
  }
}

} // namespace executors::scheduler
