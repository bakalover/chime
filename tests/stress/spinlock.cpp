#include "macro.hpp"
#include <chime/executors/full_pack.hpp>
#include <twist/test/budget.hpp>

using namespace std::chrono_literals;

TEST_SUITE(MutexLike) {
  void Test(size_t threads) {

    Pool pool{threads};
    support::SpinLock spinlock;
    size_t counter = 0, nIter = 17;

    pool.Start();

    for (size_t i = 0; i < threads; ++i) {
      Submit(pool, [&] {
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

    pool.WaitIdle();
    pool.Stop();
    std::cout << "Critical sections: " << counter << std::endl;
  }

  TWIST_STRESS_TEST(Stress3, 5s) { Test(3); }

  TWIST_STRESS_TEST(Stress3, 5s) { Test(4); }
}