#pragma once

#include <chime/executors/scheduler/hint.hpp>
#include <chime/executors/task.hpp>
#include <chime/executors/scheduler/queues/work_stealing_queue.hpp>
#include <chime/executors/scheduler/metrics.hpp>

#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/stdlike/thread.hpp>

#include <cstdlib>
#include <optional>
#include <random>
#include <span>

namespace executors {

class Scheduler;

class Worker {
private:
#if !defined(TWIST_FAULTY)
  static const size_t kLocalQueueCapacity = 256;
#else
  static const size_t kLocalQueueCapacity = 17;
#endif

public:
  Worker(Scheduler &host, size_t index);

  void Start();
  void Join();

  // Single producer
  void Push(TaskBase *, SchedulerHint);

  // Steal from this worker
  size_t StealTasks(std::span<TaskBase *> out_buffer);

  // Wake parked worker
  void Wake();

  static Worker *Current();

  WorkerMetrics Metrics() const { return metrics_; }

  Scheduler &Host() const { return host_; }

private:
  // Use in Push
  void PushToLifoSlot(TaskBase *task);
  void PushToLocalQueue(TaskBase *task);
  void OffloadTasksToGlobalQueue(TaskBase *overflow);

  // Use in TryPickTask
  TaskBase *TryPickTaskFromLifoSlot();
  TaskBase *TryStealTasks(size_t series);
  TaskBase *TryGrabTasksFromGlobalQueue();

  // Use in PickTask
  TaskBase *TryPickTask();
  TaskBase *TryPickTaskBeforePark();

  // Or park thread
  TaskBase *PickTask();

  // Run Loop
  void Work();

private:
  Scheduler &host_;
  const size_t index_;

  // Worker thread
  std::optional<twist::ed::stdlike::thread> thread_;

  // Scheduling iteration
  size_t iter_ = 0;

  // Local queue
  WorkStealingQueue<TaskBase, kLocalQueueCapacity> local_tasks_;

  // LIFO slot
  twist::ed::stdlike::atomic<TaskBase *> lifo_slot_{nullptr};

  // Deterministic pseudo-randomness for work stealing
  std::mt19937_64 twister_;

  // Parking lot
  twist::ed::stdlike::atomic<uint32_t> wakeups_{0};

  WorkerMetrics metrics_;
};

} // namespace executors