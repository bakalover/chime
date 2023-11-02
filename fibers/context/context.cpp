#include <meijic/fibers/context/context.hpp>

namespace fibers::coro
{

void ArgShift(void*, void*, void*, void*, void*, void*, void* arg7) {
  exec::Runnable* t = (exec::Runnable*)arg7;
  t->Run();
}

void Context::Setup(exec::Runnable *tramp) {
    rsp_ = setupcontext((void *)stack_.End(), (void *)ArgShift, tramp);
}

void Context::SwitchTo(Context& to){
    switchcontext(&rsp_, &to.rsp_);
}
} // namespace fibers::coro::context
