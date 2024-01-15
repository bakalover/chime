#include <chime/executors/full_pack.hpp>
#include <chime/support/test/macro.hpp>
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

  STRESS_TEST(Stress2, 5s) { Test(2); }

  STRESS_TEST(Stress5, 5s) { Test(5); }
}