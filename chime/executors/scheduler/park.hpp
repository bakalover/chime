#pragma once
#include <cstdint>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/wait/futex.hpp>
#include <twist/rt/layer/facade/wait/futex.hpp>
namespace executors::scheduler {

class ParkingLot {
public:
  uint32_t Prepare();
  void ParkIfInEpoch(uint32_t epoch);
  void Unpark();

private:
  void StepIntoNewEpoch();

private:
  twist::ed::stdlike::atomic<uint32_t> epoch_{0};
};

} // namespace executors::scheduler