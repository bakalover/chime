#include <cassert>
#include <meijic/fibers/fiber.hpp>
#include <meijic/fibers/handle.hpp>
#include <utility>

namespace fib {

Fiber *FiberHandle::Release() {
  assert(IsValid());
  return std::exchange(fiber_, nullptr);
}

void FiberHandle::Schedule() { Release()->Schedule(); }

void FiberHandle::Switch() { Release()->Switch(); }

} // namespace fib
