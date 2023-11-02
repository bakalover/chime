#pragma once
#include <meijic/executors/run.hpp>
#include <cstddef>
#include <meijic/fibers/context/stack.hpp>

namespace fibers::coro {

extern "C" void *setupcontext(void *rsp_, void *tramp, void *coro);
extern "C" void switchcontext(void* from,void* to);

class Context {
public:
    Context() : stack_(65536) {}
    
    ~Context() = default;

   void Setup(exec::Runnable* tramp);

   void SwitchTo(Context &to);

 private:
   // TODO: More Abstracitons / mb ResourseManager
   Stack stack_;
   void *rsp_;
};
}