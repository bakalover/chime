#include <chime/support/spinlock.hpp>
#include <chime/support/test/macro.hpp>
#include <cstddef>
#include <twist/ed/stdlike/thread.hpp>
#include <wheels/test/framework.hpp>

TEST_SUITE(ScalableSpinLock) {
  TWIST_UNIT_TEST(LockUnlock) {
    support::SpinLock spinlock;
    { support::SpinLock::Guard guard{spinlock}; }
  }

  TWIST_UNIT_TEST(Sequential) {
    support::SpinLock spinlock;

    { support::SpinLock::Guard guard{spinlock}; }
    { support::SpinLock::Guard guard{spinlock}; }
    { support::SpinLock::Guard guard{spinlock}; }
  }

  TWIST_UNIT_TEST(ConcurrentIncrements) {
    support::SpinLock spinlock;

    size_t counter = 0;

    const size_t nIter = 3000;

    auto f = [&] {
      for (size_t i = 0; i < nIter; ++i) {
        {
          support::SpinLock::Guard guard{spinlock};
          ++counter;
        }
      }
    };

    twist::ed::stdlike::thread t1{f};
    twist::ed::stdlike::thread t2{f};
    twist::ed::stdlike::thread t3{f};

    t1.join();
    t2.join();
    t3.join();

    ASSERT_EQ(counter, 3 * nIter);
  }
}