#include <chime/executors/executor.hpp>
#include <chime/fibers/awaiter.hpp>
#include <chime/fibers/sched/suspend.hpp>
#include <chime/fibers/sched/teleport.hpp>

namespace fibers::sched {
struct TeleportAwaiter : IAwaiter {

  explicit TeleportAwaiter(executors::IExecutor *scheduler)
      : scheduler_(scheduler) {}

  void AwaitSuspend(FiberHandle handle) override {
    handle.ScheduleVia(scheduler_);
  }

  executors::IExecutor *scheduler_;
};

void TeleportTo(executors::IExecutor *to) {
  TeleportAwaiter awaiter{to};
  Suspend(&awaiter);
}
} // namespace fibers::sched