#include <meijic/fibers/fiber.hpp>
namespace fibers {

Fiber::Fiber(exec::IExecutor &sched, exec::ITask *routine)
    : core_(routine), sched_(sched) {}

void Fiber::ReSchedule() {

}

void Fiber::Yield() { this->core_.Suspend(); }

Fiber *Fiber::Self() { return me; }

} // namespace fibers