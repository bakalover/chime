#include "meijic/fibers/sched/spawn.hpp"
#include "meijic/fibers/sync/mutex.hpp"
#include "twist/run/cross.hpp"
#include <meijic/fibers/sync.hpp>
#include <mutex>
int main() {

  twist::run::Cross([] {
    fibers::sync::Mutex mutex;
    for (size_t i = 0; i < 4; ++i) {
    }
  });
}