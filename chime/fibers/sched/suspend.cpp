#include <chime/fibers/sched/suspend.hpp>
#include <chime/fibers/fiber.hpp>

namespace fibers::sched {

void Suspend(IAwaiter* awaiter) {
  if (Fiber::InContext()) {
    Fiber::Self()->Suspend(awaiter);
  }
}

}  // namespace fib
