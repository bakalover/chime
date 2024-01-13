#pragma once

#include <chime/futures/thunks/flatten.hpp>
#include <chime/futures/traits/value_of.hpp>

namespace futures {

namespace pipe {

struct [[nodiscard]] Flatten {
  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Flatten{std::move(input)};
  }
};

} // namespace pipe

inline auto Flatten() { return pipe::Flatten{}; }

} // namespace futures
