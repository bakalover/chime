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

TaskBase *Worker::PickTask() {
  // Poll in order:
  // * [%61] Global queue
  // * LIFO slot
  // * Local queue
  // * Global queue
  // * Work stealing
  // Then
  //   Park worker

  return nullptr; // Not implemented
}

void Worker::Work() {
  // Not implemented

  while (TaskBase *next = PickTask()) {
    next->Run();
  }
}

void Worker::PushToLifoSlot(TaskBase *task) {
  TaskBase *some = lifo_slot_.exchange(task);
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
  TaskBase *offload_buffer[kLocalQueueCapacity];

  local_tasks_.Grab({offload_buffer, offload_size});
  host_.global_tasks_.Offload(offload_buffer);
}

} // namespace executors
