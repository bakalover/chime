#include <chime/fibers/fiber.hpp>
#include <chime/fibers/sched/suspend.hpp>
#include <chime/fibers/sched/yield.hpp>

namespace fibers {
struct YieldAwaiter : IAwaiter {
  void AwaitSuspend(FiberHandle handle) override { handle.Schedule(); }
};

void Yield() {
  YieldAwaiter awaiter;
  Suspend(&awaiter);
}
} // namespace fibers::sched