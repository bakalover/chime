#pragma once

#include <meijic/futures/thunks/flatten.hpp>
#include <meijic/futures/traits/value_of.hpp>

namespace futures {

namespace pipe {

struct [[nodiscard]] Flatten {
  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Flatten{std::move(input)};
  }
};

} // namespace pipe

// Future<Future<T>> -> Future<T>

inline auto Flatten() { return pipe::Flatten{}; }

} // namespace futures
