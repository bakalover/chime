#include "meijic/executors/run.hpp"
#include <meijic/fibers/coro.hpp>

namespace fibers::coro
{

    Coroutine::Coroutine(exec::Runnable* runnable){
        callee_.Setup(this);
    }

    void Coroutine::Run() noexcept{
        routine_->Run();
        Complete();
        std::abort();
    }

    void Coroutine::Resume() noexcept{
        caller_.SwitchTo(callee_);
    }

    void Coroutine::Suspend() {
        callee_.SwitchTo(caller_);
    }

    void Coroutine::Complete() {
        is_complete_ = true;
        callee_.SwitchTo(caller_);
        std::abort();
    }
} // namespace fibers::coro
