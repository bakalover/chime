#pragma once

#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/boxed.hpp>
#include <utility>

namespace futures {

namespace pipe {

struct [[nodiscard]] Box {
  template <SomeFuture InputFuture>
  BoxedFuture<traits::ValueOf<InputFuture>> Pipe(InputFuture input) {
    return thunks::Boxed<traits::ValueOf<InputFuture>>{std::move(input)};
  }
};

} // namespace pipe

inline auto Box() { return pipe::Box{}; }

} // namespace futures
