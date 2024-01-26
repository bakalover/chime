#include "macro.hpp"
#include "twist/ed/stdlike/thread.hpp"
#include "twist/rt/run.hpp"
#include <chime/support/spinlock.hpp>
#include <twist/test/budget.hpp>
#include <vector>

using namespace std::chrono_literals;

TEST_SUITE(MutexLike) {
  void Test(size_t threads) {

    support::SpinLock spinlock;
    size_t counter = 0;

    std::vector<twist::ed::stdlike::thread> contenders;

    for (size_t i = 0; i < threads; ++i) {
      contenders.emplace_back([&] {
        while (twist::test::KeepRunning()) {
          {
            {
              support::SpinLock::Guard guard{spinlock};
              ++counter;
            }
          }
        }
      });
    }

    for (auto &t : contenders) {
      t.join();
    }
    
    std::cout << "Critical sections: " << counter << std::endl;
  }

  TWIST_STRESS_TEST(Stress3, 5s) { Test(3); }

  TWIST_STRESS_TEST(Stress5, 5s) { Test(5); }
}