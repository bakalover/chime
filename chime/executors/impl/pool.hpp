#pragma once
#include <cassert>
#include <cstddef>
#include <chime/executors/executor.hpp>
#include <chime/executors/task.hpp>
#include <chime/support/group.hpp>
#include <chime/support/spinlock.hpp>
#include <chime/support/queues/unlim.hpp>
#include <thread>
#include <twist/ed/std/atomic.hpp>
#include <twist/ed/std/thread.hpp>
#include <vector>

namespace executors {

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
  support::queue::MPMCUnlimitedQueue<TaskBase> queue_;
  std::vector<twist::ed::std::thread> workers_;
  support::Group group_;
  twist::ed::std::atomic<bool> is_processing_{false};
  size_t threads_number_;
  support::SpinLock spin_;
};

} // namespace executors