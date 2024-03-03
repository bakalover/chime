#include <chime/support/group.hpp>
#include <cstddef>
#include <twist/ed/stdlike/thread.hpp>
#include <vector>
#include <wheels/test/framework.hpp>

TEST_SUITE(WaitGroup) {
  SIMPLE_TEST(JustWork) {
    support::WaitGroup wg;
    wg.Add(2);
    wg.Done();
    wg.Done();
    wg.Wait();
  }

  SIMPLE_TEST(Reverse) {
    support::WaitGroup wg;
    wg.Done();
    wg.Done();
    wg.Add(2);
    wg.Wait();
  }

  SIMPLE_TEST(ZeroNoWait) {
    support::WaitGroup wg;
    wg.Wait();
    wg.Wait();
    wg.Wait();
  }

  SIMPLE_TEST(AwaitOther) {

    const size_t nPasses = 61;
    support::WaitGroup wg;

    wg.Add(nPasses);

    std::vector<twist::ed::stdlike::thread> threads;

    for (size_t i = 0; i < nPasses; ++i) {
      threads.emplace_back([&] { wg.Done(); });
    }

    wg.Wait();

    wg.Add(1); // And one more, just for sure

    threads.emplace_back([&] { wg.Wait(); });

    wg.Done();

    for (auto &t : threads) {
      t.join();
    }
  }
}