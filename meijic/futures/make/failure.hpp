#pragma once

#include <meijic/futures/thunks/error.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/types/error.hpp>

namespace futures {

template <typename T> Future<T> auto Failure(result::Error with) {
  return thunks::Failure<T>{std::move(with)};
}

} // namespace futures
