#include "meijic/executors/impl/manual.hpp"
#include "meijic/result/types/error.hpp"
#include "meijic/result/types/status.hpp"
#include "twist/run/cross.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <meijic/fibers/sync.hpp>
#include <meijic/futures/futures.hpp>
#include <mutex>
int main() {
  twist::run::Cross([] {
    executors::ManualExecutor manual1;
    executors::ManualExecutor manual2;
    using Unit = result::Unit;
    using Status = result::Status;
    using Error = result::Error;
    size_t steps = 0;

    futures::Just() | futures::Via(manual1) | futures::Map([&](Unit) {
      ++steps;
      return Unit{};
    }) | futures::AndThen([&](Unit) -> Status {
      ++steps;
      return result::Ok();
    }) | futures::Map([](Unit) { return Unit{}; }) |
        futures::Via(manual2) | futures::Map([&](Unit) {
          ++steps;
          return Unit{};
        }) |
        futures::OrElse([&](Error) -> Status {
          std::abort();
          return result::Ok();
        }) |
        futures::Map([&](Unit) {
          ++steps;
          return Unit{};
        })

        | futures::Via(manual1) | futures::Map([&](Unit) {
            ++steps;
            return Unit{};
          }) |
        futures::FlatMap([&](Unit) {
          ++steps;
          return futures::Value(1);
        }) |
        futures::Map([&](int v) {
          assert(v == 1);
          ++steps;
          return Unit{};
        }) |
        futures::Detach();

    {
      size_t tasks = manual1.Drain();
      assert(tasks == 3);
    }

    assert(steps == 2);

    {
      size_t tasks = manual2.Drain();
      assert(tasks >= 2);
    }

    assert(steps == 4);

    {
      size_t tasks = manual1.Drain();
      assert(tasks >= 3);
    }
    assert(steps == 7);
  });
}