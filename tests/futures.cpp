#include "meijic/executors/impl/manual.hpp"
#include "meijic/executors/impl/pool.hpp"
#include "meijic/futures/combine/seq/and_then.hpp"
#include "meijic/futures/combine/seq/flatten.hpp"
#include "meijic/futures/combine/seq/map.hpp"
#include "meijic/futures/make/value.hpp"
#include "meijic/futures/run/detach.hpp"
#include "meijic/futures/run/get.hpp"
#include "meijic/result/make/ok.hpp"
#include "meijic/result/types/error.hpp"
#include "meijic/result/types/result.hpp"
#include "meijic/result/types/status.hpp"
#include "meijic/result/types/unit.hpp"
#include "twist/ed/std/atomic.hpp"
#include "twist/run/cross.hpp"
#include "twist/test/budget.hpp"
#include "twist/test/repeat.hpp"
#include "wheels/test/runtime.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <meijic/fibers/sync.hpp>
#include <meijic/futures/full_pack.hpp>
#include <mutex>
#include <utility>
using std::chrono_literals::operator""s;
int main() {
  twist::run::Cross([] {
    executors::Pool pool1{4};
    executors::Pool pool2{4};
    pool1.Start();
    pool2.Start();
    auto fut = Just() | Via(pool1) |
               AndThen([&](result::Unit) { return result::Ok(Value(1)); }) |
               Flatten() | FlatMap([&](int val) {
                 twist::ed::std::atomic<size_t> counter = 0;

                 for (size_t i = 0; i < 1000000; ++i) {

                   Submit(pool2, [&] {
                     counter.fetch_add(1);
                     return result::Ok();
                   }) | Detach();
                 }
                 pool2.WaitIdle();
                 return Value(counter.load());
               }) |
               Flatten();
    auto res = move(fut) | Get();
    pool1.WaitIdle();
    pool2.WaitIdle();
    pool1.Stop();
    pool2.Stop();
    std::cout << *res << std::endl;
  });
}