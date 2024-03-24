#include "twist/rt/layer/facade/wait/futex.hpp"
#include <chime/executors/scheduler/park.hpp>

namespace executors::scheduler {
void ParkingLot::Park() { twist::ed::futex::Wait(epoch_, epoch_.load()); }
void ParkingLot::Unpark() {
  auto key = twist::ed::futex::PrepareWake(epoch_);
  epoch_.fetch_add(1);
  twist::ed::futex::WakeOne(key);
}

} // namespace executors::scheduler