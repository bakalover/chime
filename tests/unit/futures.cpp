#include "chime/executors/impl/pool/pool.hpp"
#include "chime/futures/combine/seq/start.hpp"
#include "chime/futures/make/just.hpp"
#include "chime/futures/make/submit.hpp"
#include "chime/result/make/ok.hpp"
#include <chime/futures/full_pack.hpp>
#include <chime/support/group.hpp>
#include <cstddef>
#include <twist/ed/stdlike/thread.hpp>
#include <utility>
#include <vector>
#include <wheels/test/framework.hpp>

TEST_SUITE(WaitGroup) {
  SIMPLE_TEST(JustWork) {
    executors::Pool pool{8};
    pool.Start();

    auto fast = Submit(pool, [] {
      size_t count = 0;
      for (size_t i = 0; i < 100; ++i) {
        ++count;
      }
      return result::Ok(count);
    });

    auto medium = Submit(pool, [] {
      size_t count = 0;
      for (size_t i = 0; i < 100000; ++i) {
        ++count;
      }
      return result::Ok(count);
    });

    auto slow = Submit(pool, [] {
      size_t count = 0;
      for (size_t i = 0; i < 1000000000; ++i) {
        ++count;
      }
      return result::Ok(count);
    });

    // auto first = First(move(fast), move(medium), move(slow));
    auto first = First(Just(), Just(), Just());
  }
}