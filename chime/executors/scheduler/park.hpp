#pragma once
#include <cstdint>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/wait/futex.hpp>
#include <twist/rt/layer/facade/wait/futex.hpp>
namespace executors::scheduler {

class ParkingLot {

public:
  using Epoch = uint32_t;

  Epoch AnounceEpoch();
  void ParkIfInEpoch(Epoch epoch);
  void Unpark();

private:
  void StepIntoNewEpoch();

private:
  twist::ed::stdlike::atomic<Epoch> epoch_{0};
};

} // namespace executors::scheduler