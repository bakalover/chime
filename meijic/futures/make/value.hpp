#pragma once

#include <meijic/futures/thunks/value.hpp>
#include <meijic/futures/types/future.hpp>
#include <utility>

namespace futures {

template <typename T> Future<T> auto Value(T value) {
  return thunks::Value<T>{std::move(value)};
}

} // namespace futures
