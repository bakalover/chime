#pragma once

#include <meijic/futures/model/consumer.hpp>

namespace futures {

// https://wiki.haskell.org/Thunk

template <typename T>
concept Thunk = requires(T thunk, IConsumer<typename T::ValueType>* consumer) {
  typename T::ValueType;

  thunk.Start(consumer);
};

}  // namespace meijic::futures
