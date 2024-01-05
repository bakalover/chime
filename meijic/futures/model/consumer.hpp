#pragma once

#include <meijic/futures/model/context.hpp>
#include <meijic/futures/model/output.hpp>
#include <utility>

namespace futures {

template <typename T> struct IConsumer {
  virtual ~IConsumer() = default;

  // Invoked by producer

  void Complete(result::Result<T> r) noexcept {
    Consume({std::move(r), Context{}});
  }

  void Complete(Output<T> o) noexcept { Consume(std::move(o)); }

  // Overriden by consumer
  virtual void Consume(Output<T>) noexcept = 0;
};

} // namespace futures
