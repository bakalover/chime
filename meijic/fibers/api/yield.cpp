#include <meijic/fibers/api/yield.hpp>
#include <meijic/fibers/fiber.hpp>
namespace fibers {
void Yield() {
  Fiber *cur_fiber = Fiber::Self();
  if (cur_fiber != nullptr) {
    cur_fiber->Yield();
  }
}
} // namespace fibers