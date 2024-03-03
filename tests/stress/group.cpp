#include <chime/support/group.hpp>
#include <cstddef>
#include <tests/stress/macro.hpp>
#include <twist/ed/stdlike/thread.hpp>
#include <twist/test/budget.hpp>
#include <twist/test/repeat.hpp>
#include <vector>
#include <wheels/test/framework.hpp>

void Test() {
  const size_t nProducers = 3;

  std::vector<twist::ed::stdlike::thread> producers;
  support::WaitGroup wg;

  std::array<bool, nProducers> check;
  check.fill(false);

  for (size_t i = 0; i < nProducers; ++i) {
    producers.emplace_back([&, i] {
      check[i] = true;
      wg.Done();
    });
  }

  wg.Wait();

  for (size_t i = 0; i < nProducers; ++i) {
    ASSERT_TRUE(check[i]);
  }
}

TEST_SUITE(WaitGroup) {
  TWIST_TEST_REPEAT(ProduceConsume, 5s) { Test(); }
}