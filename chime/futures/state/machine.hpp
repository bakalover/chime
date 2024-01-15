#pragma once
#include <cstdint>
#include <cstdlib>
#include <twist/ed/stdlike/atomic.hpp>
namespace futures::state {
class StateMachine {
private:
  using State = uint8_t;

  struct States {
    enum _ {
      Init = 0,
      HasProducer = 1,
      HasConsumer = 2,
      Done = HasConsumer | HasProducer
    };
  };

public:
  bool Produce() {
    switch (state_.fetch_or(States::HasProducer, std::memory_order::acq_rel)) {
    case States::Init:
      return false;
    case States::HasConsumer:
      return true;
    default:
      std::abort();
    }
  }
  bool Consume() {
    switch (state_.fetch_or(States::HasConsumer, std::memory_order::acq_rel)) {
    case States::Init:
      return false;
    case States::HasProducer:
      return true;
    default:
      std::abort();
    }
  }

private:
  twist::ed::stdlike::atomic<State> state_{States::Init};
};
} // namespace futures::state