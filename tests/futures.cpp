#include "chime/fibers/sched/spawn.hpp"
#include "chime/fibers/sched/yield.hpp"
#include "chime/futures/combine/seq/map.hpp"
#include "chime/futures/combine/seq/start.hpp"
#include "chime/futures/combine/seq/via.hpp"
#include <chime/executors/impl/pool.hpp>
#include <chime/fibers/sched.hpp>
#include <chime/futures/full_pack.hpp>
#include <iostream>
#include <utility>
int main() {
  executors::Pool pool{1};

  pool.Start();

  Spawn(pool, [&] {
    std::cout << "1\n";
    Spawn(pool, [&] {
      std::cout << "2\n";
      auto f = Just() | Via(pool) | Map([](result::Unit) { return "hello"; }) |
               Bang();
      Yield();

      std::cout << *(std::move(f) | Get()) << '\n';
      std::cout << "4\n";
    });
    Yield();
    std::cout << "3\n";
  });

  pool.WaitIdle();
  pool.Stop();
}