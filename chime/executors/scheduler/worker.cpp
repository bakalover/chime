#include <atomic>
#include <chime/executors/scheduler/park.hpp>
#include <chime/executors/scheduler/scheduler.hpp>
#include <chime/executors/scheduler/worker.hpp>
#include <chime/executors/tasks/task.hpp>
#include <cstddef>
#include <twist/ed/local/ptr.hpp>
#include <twist/rt/layer/strand/local/ptr.hpp>

namespace executors::scheduler {

TWISTED_THREAD_LOCAL_PTR(Worker, curr_worker)

Worker::Worker(Scheduler &host, size_t index) : host_{host}, index_{index} {}

void Worker::Start() {
  Host().wg_.Add(); // Track worker activity
  thread_.emplace([this]() { Work(); });
}

void Worker::Join() { thread_->join(); }

Worker *Worker::Current() { return curr_worker; }

bool Worker::InContext(Scheduler *exe) {
  auto worker = Worker::Current();
  return worker != nullptr && exe == &worker->Host();
}

size_t Worker::StealTasks(std::span<TaskBase *> out_buffer) {
  return local_tasks_.Grab(out_buffer);
}

void Worker::PushWithStrategy(TaskBase *task, SchedulerHint hint) {
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
  TaskBase *task = host_.global_tasks_.TryPop();
  if (task != nullptr) {
    Host().wg_.Done(); // Track global task
  }
  return task;
}

TaskBase *Worker::TryPickTaskFromLifoSlot() {
  return lifo_slot_.exchange(nullptr, std::memory_order_acquire);
}

TaskBase *Worker::TryPickTaskFromLocalQueue() { return local_tasks_.TryPop(); }

TaskBase *Worker::TryGrabTasksFromGlobalQueue() {
  size_t to_grab = kLocalQueueCapacity - local_tasks_.Size();
  size_t actual_grab =
      host_.global_tasks_.Grab({tranfer_buffer_, to_grab}, host_.threads_);

  Host().wg_.Done(actual_grab); // Track Global task

  if (actual_grab > 0) {
    TaskBase *next = tranfer_buffer_[0];

    // Maybe zero
    local_tasks_.PushMany({tranfer_buffer_ + 1, actual_grab - 1});
    return next;
  }
  return nullptr;
}

TaskBase *Worker::TryStealTasks(size_t series) {
  TaskBase **buff_addr = &tranfer_buffer_[0];

  size_t steal_count = 5;
  size_t actually_stolen = 0;

  for (size_t i = 0; i < series; ++i) {
    if (steal_count == 0) {
      break;
    } else {
      for (size_t j = 0; j < host_.threads_; ++j) {
        if (steal_count == 0) {
          break;
        } else {
          size_t target = (twister_() + i) % host_.threads_;
          size_t stolen =
              host_.workers_[target].StealTasks({buff_addr, steal_count});
          steal_count -= stolen;
          buff_addr += stolen;
          actually_stolen += stolen;
        }
      }
    }
  }
  if (actually_stolen == 0) {
    return nullptr;
  }
  if (actually_stolen != 1) {
    local_tasks_.PushMany({&tranfer_buffer_[0] + 1, actually_stolen - 1});
  }
  return buff_addr[0];
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

  // Stealing (we are STARVING)
  if (host_.coordinator_.TrySpin()) {

    static const size_t series = 4;
    TaskBase *next = TryStealTasks(series);

    bool is_last = host_.coordinator_.StopSpin();
    if (next != nullptr && is_last) {
      host_.coordinator_.WakeOne();
    }
    return next;
  }

  return nullptr;
}

TaskBase *Worker::TryPickTaskBeforePark() {
  TaskBase *next;

  if ((next = TryPickTaskFromGlobalQueue()) != nullptr) {
    return next;
  }

  static const size_t beforeParkSeries = 1;

  if ((next = TryStealTasks(beforeParkSeries)) != nullptr) {
    return next;
  }

  return nullptr;
}

TaskBase *Worker::PickTask() {
  TaskBase *next;

  while (Host().coordinator_.IsNotShutDowned()) {

    if ((next = TryPickTask()) != nullptr) {
      return next;
    }

    ParkingLot::Epoch epoch = parking_lot_.AnnounceEpoch();

    host_.coordinator_.BecomeIdle(this);

    if ((next = TryPickTaskBeforePark()) != nullptr) {
      host_.coordinator_.BecomeActive();
      return next;
    }

    if (Host().coordinator_.IsShutDowned()) {
      host_.coordinator_.BecomeActive();
      return nullptr;
    }

    Host().wg_.Done(); // Track worker activity
    parking_lot_.ParkIfInEpoch(epoch);
    Host().wg_.Add(); // Track worker activity
  }
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
  Host().wg_.Add(grab_batch_size); // Track Global tasks
  host_.global_tasks_.Offload({tranfer_buffer_, grab_batch_size});
}

void Worker::Wake() { parking_lot_.Unpark(); }

void Worker::Work() {

  twister_.seed(host_.random_());

  while (TaskBase *next = PickTask()) {
    next->Run();
    Host().wg_.Done(); // Track Worker Activity
  }
}
} // namespace executors::scheduler
