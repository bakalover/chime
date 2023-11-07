
#include <meijic/fibers/coro.hpp>
namespace fibers::coro {

Coroutine::Coroutine(exec::ITask *routine)
    : stack_(sure::Stack::AllocateBytes(65536)) {
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
  std::abort();
}
} // namespace fibers::coro
