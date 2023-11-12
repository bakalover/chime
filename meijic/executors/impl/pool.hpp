#pragma once
#include "meijic/support/locks/spinlock.hpp"
#include <atomic>
#include <cassert>
#include <cstddef>
#include <meijic/executors/executor.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/support/group.hpp>
#include <meijic/support/queues/unlim.hpp>
#include <thread>
namespace exe {

// Fixed-size pool of worker threads

class Pool : public IExecutor {
public:
  explicit Pool(size_t threads);

  ~Pool();

  // Non-copyable
  Pool(const Pool &) = delete;
  Pool &operator=(const Pool &) = delete;

  // Non-movabletwist::ed::
  Pool(Pool &&) = delete;
  Pool &operator=(Pool &&) = delete;

  // Launches worker threads
  void Start();

  // Schedules task for execution in one of the worker threads
  void Submit(TaskBase *task);

  // Locates current thread pool from worker thread
  static Pool *Current();

  // Waits until outstanding work count reaches zero
  void WaitIdle();

  // Stops the worker threads as soon as possible
  void Stop();

private:
  void StartWorker(Pool *owner);

  void WorkerProcess();

  size_t AvailableCores();

  void PackWorkers();

private:
  sup::queue::MPMCUnlimitedQueue<TaskBase> queue_;
  std::vector<std::thread> workers_;
  sup::Group group_;
  std::atomic<bool> is_processing_{false};
  size_t threads_number_;
  sup::SpinLock spin_;
};

} // namespace exe