#include <meijic/executors/executor.hpp>
#include <meijic/fibers/awaiter.hpp>
#include <meijic/fibers/sched/suspend.hpp>
#include <meijic/fibers/sched/teleport.hpp>

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