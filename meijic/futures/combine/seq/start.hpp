#pragma once

#include <meijic/futures/state/eager_runner.hpp>
#include <meijic/futures/state/state.hpp>
#include <meijic/futures/thunks/eager.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/eager.hpp>
#include <memory>
#include <utility>

namespace futures {

namespace pipe {

struct [[nodiscard]] Start {
  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    using Runner = state::EagerRunner<InputFuture>;
    auto runner = std::make_shared<Runner>(std::move(input));
    runner->RunThunk();
    return thunks::Eager<traits::ValueOf<InputFuture>>{std::move(runner)};
  }
};

} // namespace pipe

// Future<T> -> EagerFuture<T>

inline auto Start() { return pipe::Start{}; }

inline auto Bang() { return pipe::Start{}; }

} // namespace futures
