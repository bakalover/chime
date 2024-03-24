#include <chime/executors/scheduler/coordinator.hpp>
#include <chime/executors/scheduler/park.hpp>
#include <twist/rt/layer/facade/wait/futex.hpp>

namespace executors::scheduler {

uint32_t ParkingLot::Prepare() { return epoch_.load(); }

void ParkingLot::StepIntoNewEpoch() { epoch_.fetch_add(1); }

void ParkingLot::ParkIfInEpoch(uint32_t epoch) {
  twist::ed::futex::Wait(epoch_, epoch);
}

void ParkingLot::Unpark() {
  auto key = twist::ed::futex::PrepareWake(epoch_);
  StepIntoNewEpoch();
  twist::ed::futex::WakeOne(key);
}

} // namespace executors::scheduler