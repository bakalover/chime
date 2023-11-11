#include <meijic/fibers/sched/via.hpp>
namespace fib {
Spawner *Via(exe::IExecutor &scheduler) { return new Spawner(scheduler); }
} // namespace fibers