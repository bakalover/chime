#include "chime/executors/task.hpp"
#include <atomic>
#include <chime/executors/scheduler/scheduler.hpp>
#include <chime/executors/scheduler/worker.hpp>
#include <cstddef>
#include <twist/ed/local/ptr.hpp>
#include <twist/rt/layer/strand/local/ptr.hpp>

namespace executors {

TWISTED_THREAD_LOCAL_PTR(Worker, curr_worker)

Worker::Worker(Scheduler &host, size_t index) : host_{host}, index_{index} {}

void Worker::Start() {
  thread_.emplace([this]() { Work(); });
}

void Worker::Join() { thread_->join(); }

Worker *Worker::Current() { return curr_worker; }

size_t Worker::StealTasks(std::span<TaskBase *> out_buffer) {
  return local_tasks_.Grab(out_buffer);
}

void Worker::Push(TaskBase *task, SchedulerHint hint) {
  switch (hint) {
  case SchedulerHint::UpToYou:
    PushToLocalQueue(task);
    break;
  case SchedulerHint::Next:
    PushToLifoSlot(task);
    break;
  }
}

bool Worker::NextIter() { return ++iter_ % 61 == 0; }

TaskBase *Worker::TryPickTaskFromGlobalQueue() {
  return host_.global_tasks_.TryPop();
}

TaskBase *Worker::TryPickTaskFromLifoSlot() {
  return lifo_slot_.exchange(nullptr, std::memory_order_release);
}

TaskBase *Worker::TryPickTaskFromLocalQueue() { return local_tasks_.TryPop(); }

TaskBase *Worker::TryGrabTasksFromGlobalQueue() {
  size_t to_grab = kLocalQueueCapacity - local_tasks_.Size();
  size_t actual_grab =
      host_.global_tasks_.Grab({tranfer_buffer_, to_grab}, host_.threads_);

  if (actual_grab > 0) {
    TaskBase *next = tranfer_buffer_[0];

    // Maybe zero
    local_tasks_.PushMany({tranfer_buffer_ + 1, actual_grab - 1});
    return next;
  }
  return nullptr;
}

TaskBase *Worker::TryPickTask() {
  TaskBase *next;

  //[% 61] Global queue (only one task)
  if (NextIter()) {
    if ((next = TryPickTaskFromGlobalQueue()) != nullptr) {
      return next;
    }
  }

  // * LIFO slot
  if ((next = TryPickTaskFromLifoSlot()) != nullptr) {
    return next;
  }

  // * Local queue
  if ((next = TryPickTaskFromLocalQueue()) != nullptr) {
    return next;
  }

  // * Global queue (we are starving)
  if ((next = TryGrabTasksFromGlobalQueue()) != nullptr) {
    return next;
  }

  // * Work stealing
  // Then
  //   Park worker

  return nullptr;
}

void Worker::PushToLifoSlot(TaskBase *task) {
  TaskBase *some = lifo_slot_.exchange(task, std::memory_order_release);
  if (some != nullptr) {
    PushToLocalQueue(some);
  }
}

void Worker::PushToLocalQueue(TaskBase *task) {
  if (!local_tasks_.TryPush(task)) {
    OffloadTasksToGlobalQueue(task);
  }
}

void Worker::OffloadTasksToGlobalQueue(TaskBase *overflow) {
  size_t offload_size = local_tasks_.Size() / 2 + 1;
  size_t grab_batch_size = local_tasks_.Grab({tranfer_buffer_, offload_size});
  tranfer_buffer_[grab_batch_size++] = overflow;
  host_.global_tasks_.Offload({tranfer_buffer_, grab_batch_size});
}

void Worker::Work() {

  twister_.seed(host_.random_());

  while (TaskBase *next = PickTask()) {
    next->Run();
  }
}
} // namespace executors
