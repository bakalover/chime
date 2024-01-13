#pragma once
#include <chime/executors/executor.hpp>
#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/state/machine.hpp>
#include <optional>
namespace futures::state {

template <typename T> class Rendezvous {
public:
  void SetOutput(Output<T> output) {
    output_.emplace(std::move(output));
    if (machine_.Produce()) {
      Fire();
    }
  }

  void SetConsumer(IConsumer<T> *consumer) {
    consumer_ = consumer;
    if (machine_.Consume()) {
      Fire();
    }
  }

private:
  void Fire() { consumer_->Consume(std::move(*output_)); }

private:
  std::optional<Output<T>> output_;
  StateMachine machine_;
  IConsumer<T> *consumer_;
};

} // namespace futures::state