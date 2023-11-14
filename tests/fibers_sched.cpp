#include "meijic/fibers/sched/spawn.hpp"
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <meijic/executors/impl/manual.hpp>
#include <meijic/executors/impl/pool.hpp>
#include <meijic/executors/task.hpp>
#include <meijic/fibers/sched.hpp>
#include <meijic/support/queues/steal.hpp>
#include <ostream>
#include <thread>
using namespace std::chrono_literals;
void manual() {
  exe::ManualExecutor manual1;
  exe::ManualExecutor manual2;

  size_t step_counter = 0;

  fib::SpawnVia(manual1, [&] {
    std::cout << "Step1" << std::endl;
    ++step_counter;
    fib::Yield();
    std::cout << "Step3" << std::endl;
    ++step_counter;
  });
  fib::SpawnVia(manual1, [&] {
    std::cout << "Step2" << std::endl;
    ++step_counter;
    fib::Yield();
    std::cout << "Step4" << std::endl;
    ++step_counter;
  });
  fib::SpawnVia(manual2, [&] {
    std::cout << "Step1" << std::endl;
    ++step_counter;
    fib::Yield();
    std::cout << "Step3" << std::endl;
    ++step_counter;
  });
  fib::SpawnVia(manual2, [&] {
    std::cout << "Step2" << std::endl;
    ++step_counter;
    fib::Yield();
    std::cout << "Step4" << std::endl;
    ++step_counter;
  });

  std::cout << "Warming up First Scheduler" << std::endl;
  std::this_thread::sleep_for(2s);
  manual1.Drain();

  std::cout << "Warming up Second Scheduler" << std::endl;
  std::this_thread::sleep_for(2s);
  manual2.Drain();
  assert(step_counter == 8);
  sup::Group gr{};
  gr.Add(1);
}

void pool() {
  exe::Pool pool{8};

  std::atomic<size_t> step_counter = 0;
  for (size_t i = 0; i < 10000; ++i) {
    fib::SpawnVia(pool, [&] {
      ++step_counter;
      fib::Yield();
      ++step_counter;
      fib::Yield();
      ++step_counter;
    });
  }
  pool.Start();
  pool.WaitIdle();
  pool.Stop();
  assert(step_counter == 30000);
}

int main() {
  manual();
  pool();
}