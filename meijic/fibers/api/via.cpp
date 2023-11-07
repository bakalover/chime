#include <meijic/fibers/api/via.hpp>
namespace fibers {
Spawner *Via(exec::IExecutor &scheduler) { return new Spawner(scheduler); }
} // namespace fibers