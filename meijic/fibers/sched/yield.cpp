#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/sched/yield.hpp>
namespace fib {
struct YieldAwaiter : IAwaiter {
  void AwaitSuspend(FiberHandle handle) override { handle.Schedule(); }
};

void Yield() {
  YieldAwaiter awaiter;
  auto cur_fiber = Fiber::Self();
  if (cur_fiber != nullptr) {
    cur_fiber->Suspend(&awaiter);
  }
}
} // namespace fib::sched