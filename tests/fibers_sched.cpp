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

  fib::Via(manual1)
      ->Spawn([&] {
        std::cout << "Step1" << std::endl;
        ++step_counter;
        fib::Yield();
        std::cout << "Step3" << std::endl;
        ++step_counter;
      })
      ->Spawn([&] {
        std::cout << "Step2" << std::endl;
        ++step_counter;
        fib::Yield();
        std::cout << "Step4" << std::endl;
        ++step_counter;
      })
      ->Via(manual2)
      ->Spawn([&] {
        std::cout << "Step1" << std::endl;
        ++step_counter;
        fib::Yield();
        std::cout << "Step3" << std::endl;
        ++step_counter;
      })
      ->Spawn([&] {
        std::cout << "Step2" << std::endl;
        ++step_counter;
        fib::Yield();
        std::cout << "Step4" << std::endl;
        ++step_counter;
      })
      ->RunAll();

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
  auto spawner = fib::Via(pool);
  for (size_t i = 0; i < 10000; ++i) {
    spawner->Spawn([&] {
      ++step_counter;
      fib::Yield();
      ++step_counter;
      fib::Yield();
      ++step_counter;
    });
  }
  spawner->RunAll();
  pool.Start();
  pool.WaitIdle();
  pool.Stop();
  assert(step_counter == 30000);
}

int main() {
  manual();
  pool();
}