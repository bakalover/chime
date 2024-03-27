#include <chime/executors/pool/pool.hpp>
#include <cstddef>
#include <thread>

namespace executors {

thread_local static Pool *thread_owner = nullptr;

Pool::Pool(size_t threads_number)
    : queue_(), group_(),
      threads_number_(std::min(threads_number, AvailableCores())) {}

void Pool::Start() {
  is_processing_.store(true);
  for (size_t i = 0; i < threads_number_; ++i) {
    workers_.emplace_back([this]() { StartWorker(this); });
  }
}

Pool::~Pool() { assert(!is_processing_.load()); }

void Pool::Submit(TaskBase *task, SchedulerHint) {
  group_.Add(1);
  queue_.Put(task);
}

void Pool::StartWorker(Pool *owner) {
  thread_owner = owner;
  WorkerProcess();
}

void Pool::WorkerProcess() {
  while (thread_owner->is_processing_.load(std::memory_order_acquire)) {
    auto task = thread_owner->queue_.Take();
    if (task.has_value()) {
      (task.value())->Run();
      thread_owner->group_.Done();
    }
  }
}

size_t Pool::AvailableCores() {
  return (size_t)std::thread::hardware_concurrency();
}

Pool *Pool::Current() { return thread_owner; }

void Pool::WaitIdle() { group_.Wait(); }

void Pool::Stop() {
  queue_.Close();
  is_processing_.store(false, std::memory_order_release);
  PackWorkers();
}

void Pool::PackWorkers() {
  for (auto &worker_shell : workers_) {
    worker_shell.join();
  }
}

} // namespace executors