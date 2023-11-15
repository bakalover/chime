#include <meijic/fibers/sched/suspend.hpp>
#include <meijic/fibers/fiber.hpp>

namespace fib {

void Suspend(IAwaiter* awaiter) {
  if (Fiber::IsFiber()) {
    Fiber::Self()->Suspend(awaiter);
  }
}

}  // namespace fib
