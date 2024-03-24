#pragma once

#include <chime/executors/scheduler/hint.hpp>
#include <chime/executors/scheduler/metrics.hpp>
#include <chime/executors/scheduler/park.hpp>
#include <chime/executors/scheduler/queues/work_stealing_queue.hpp>
#include <chime/executors/task.hpp>
#include <wheels/intrusive/forward_list.hpp>

#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/stdlike/thread.hpp>

#include <cstdlib>
#include <optional>
#include <random>
#include <span>

namespace executors::scheduler {

class Scheduler;

class Worker : public wheels::IntrusiveForwardListNode<Worker> {
private:
#if defined(TWIST_FAULTY)
  static const size_t kLocalQueueCapacity = 17;
#else
  static const size_t kLocalQueueCapacity = 256;
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
  // Push
  void PushToLifoSlot(TaskBase *task);
  void PushToLocalQueue(TaskBase *task);
  void OffloadTasksToGlobalQueue(TaskBase *overflow);

  // TryPickTask
  TaskBase *TryPickTaskFromLifoSlot();
  TaskBase *TryStealTasks(size_t series);
  TaskBase *TryGrabTasksFromGlobalQueue();
  TaskBase *TryPickTaskFromGlobalQueue();
  TaskBase *TryPickTaskFromLocalQueue();

  // PickTask
  TaskBase *TryPickTask();
  TaskBase *TryPickTaskBeforePark();

  // Or park thread
  TaskBase *PickTask();

  // Run Loop
  void Work();
  bool NextIter(); // true <=> (iter_ % 61) == 0

private:
  Scheduler &host_;
  const size_t index_;

  // Worker thread
  std::optional<twist::ed::stdlike::thread> thread_;

  // Scheduling iteration
  size_t iter_ = 0;

  WorkStealingQueue<TaskBase, kLocalQueueCapacity> local_tasks_;

  // Buffer for transfer tasks between queues
  TaskBase *tranfer_buffer_[kLocalQueueCapacity];

  twist::ed::stdlike::atomic<TaskBase *> lifo_slot_{nullptr};

  // Deterministic pseudo-randomness for work stealing
  std::mt19937_64 twister_;

  ParkingLot parking_lot_;

  WorkerMetrics metrics_;
};

} // namespace executors::scheduler
