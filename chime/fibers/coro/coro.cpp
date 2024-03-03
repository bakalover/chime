
#include <wheels/core/panic.hpp>
#include <chime/fibers/coro/coro.hpp>
#include <chime/fibers/routine.hpp>

namespace fibers {

Coroutine::Coroutine(IRoutine *routine)
    : stack_{sure::Stack::AllocateBytes(65536)}, routine_{routine} {
  me_.Setup(stack_.MutView(), this);
}

void Coroutine::Run() noexcept {

  routine_->Run();

  Complete();
}

void Coroutine::Resume() noexcept { caller_.SwitchTo(me_); }

void Coroutine::Suspend() { me_.SwitchTo(caller_); }

void Coroutine::Complete() {
  is_complete_ = true;
  me_.SwitchTo(caller_);
  WHEELS_PANIC("Violation coroutine lifecycle!");
}
} // namespace fibers
