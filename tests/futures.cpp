#include "chime/fibers/sched/spawn.hpp"
#include "chime/fibers/sched/yield.hpp"
#include <chime/executors/impl/pool.hpp>
#include <chime/fibers/sched.hpp>
#include <iostream>
int main() {
  executors::Pool pool{1};

  pool.Start();

  Spawn(pool, [&] {
    std::cout << "1\n";
    Spawn(pool, [] {
      std::cout << "2\n";
      Yield();
      std::cout << "4\n";
    });
    Yield();
    std::cout << "3\n";
  });

  pool.WaitIdle();
  pool.Stop();
}