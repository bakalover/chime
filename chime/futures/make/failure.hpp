#pragma once

#include <chime/futures/thunks/error.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/types/error.hpp>

namespace futures {

template <typename T> Future<T> auto Failure(result::Error with) {
  return thunks::Failure<T>{std::move(with)};
}

} // namespace futures
