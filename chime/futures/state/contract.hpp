#pragma once

#include <chime/futures/model/output.hpp>
#include <chime/futures/state/rendezvous.hpp>
#include <chime/futures/state/state.hpp>

namespace futures::state {

template <typename T> class Contract final : public IState<T> {
public:
  Contract(){};

  // Eager Future
  void SetConsumer(IConsumer<T> *consumer) override {
    rendezvous_.SetConsumer(consumer);
  }

  // Promise
  void SetOutput(Output<T> output) { rendezvous_.SetOutput(std::move(output)); }

private:
  Rendezvous<T> rendezvous_;
};
} // namespace futures::state