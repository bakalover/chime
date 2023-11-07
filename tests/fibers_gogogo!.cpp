#include <cassert>
#include <cstddef>
#include <iostream>
#include <meijic/executors/impl/manual.hpp>
#include <meijic/fibers/api/via.hpp>
#include <meijic/fibers/api/yield.hpp>
#include <ostream>
#include <thread>
using namespace std::chrono_literals;
int main() {
  exec::ManualExecutor manual1;
  exec::ManualExecutor manual2;

  size_t step_counter = 0;

  fibers::Via(manual1)
      ->Spawn([&] {
        std::cout << "Step1" << std::endl;
        ++step_counter;
        fibers::Yield();
        std::cout << "Step3" << std::endl;
        ++step_counter;
      })
      ->Spawn([&] {
        std::cout << "Step2" << std::endl;
        ++step_counter;
        fibers::Yield();
        std::cout << "Step4" << std::endl;
        ++step_counter;
      })
      ->Via(manual2)
      ->Spawn([&] {
        std::cout << "Step1" << std::endl;
        ++step_counter;
        fibers::Yield();
        std::cout << "Step3" << std::endl;
        ++step_counter;
      })
      ->Spawn([&] {
        std::cout << "Step2" << std::endl;
        ++step_counter;
        fibers::Yield();
        std::cout << "Step4" << std::endl;
        ++step_counter;
      })
      ->RunAll();
  std::cout << "Warming up First Scheduler" << std::endl;
  manual1.RunNext();
  std::this_thread::sleep_for(2s);
  manual1.Drain();
  std::cout << "Warming up Second Scheduler" << std::endl;
  manual2.RunNext();
  std::this_thread::sleep_for(2s);
  manual2.Drain();
  assert(step_counter == 8);
}