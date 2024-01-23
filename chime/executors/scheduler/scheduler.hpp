#pragma once

#include <chime/executors/executor.hpp>

#include <chime/executors/scheduler/coordinator.hpp>
#include <chime/executors/scheduler/metrics.hpp>
#include <chime/executors/scheduler/queues/global_queue.hpp>
#include <chime/executors/scheduler/worker.hpp>

// random_device
#include <twist/ed/stdlike/random.hpp>

#include <deque>

namespace executors {

// Scalable work-stealing scheduler for short-lived tasks

class Scheduler : public IExecutor {
  friend class Worker;

 public:
   explicit Scheduler(size_t threads);
   ~Scheduler();

   // Non-copyable
   Scheduler(const Scheduler &) = delete;
   Scheduler &operator=(const Scheduler &) = delete;

   void Start();

   // IExecutor
   void Submit(TaskBase *) override;

   void Stop();

   // After Stop
   PoolMetrics Metrics() const;

   static Scheduler *Current();

 private:
  const size_t threads_;
  std::deque<Worker> workers_;
  Coordinator coordinator_;
  GlobalQueue global_tasks_;
  twist::ed::stdlike::random_device random_;
  // ???
};

} // namespace executors
