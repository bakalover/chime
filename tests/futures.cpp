#include "meijic/fibers/sched/spawn.hpp"
#include "meijic/fibers/sync/mutex.hpp"
#include "meijic/futures/combine/seq/map.hpp"
#include "meijic/futures/make/just.hpp"
#include "meijic/futures/run/get.hpp"
#include "twist/run/cross.hpp"
#include <cassert>
#include <iostream>
#include <meijic/fibers/sync.hpp>
#include <mutex>
int main() {
  twist::run::Cross([] {
    auto res = futures::Just() | futures::Map([](result::Unit) { return 1; }) |
               futures::Get();
    std::cout << *res;
  });
}