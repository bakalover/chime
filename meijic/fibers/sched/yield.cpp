#include <meijic/fibers/sched/yield.hpp>
#include <meijic/fibers/fiber.hpp>
namespace fib::sched {
void Yield() {
  Fiber *cur_fiber = Fiber::Self();
  if (cur_fiber != nullptr) {
    cur_fiber->Yield();
  }
}
} // namespace fibers