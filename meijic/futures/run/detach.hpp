#pragma once

#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/output.hpp>
#include <meijic/futures/syntax/pipe.hpp>
#include <meijic/futures/thunks/grave.hpp>
#include <utility>

namespace futures {

namespace pipe {

struct [[nodiscard]] Detach {
  template <SomeFuture Future> auto Pipe(Future f) {
    // Grave will self-destruct after execution
    auto grave = new thunks::Grave<Future>{std::move(f)};
    grave->BuryToGround();
  }
};

} // namespace pipe

inline auto Detach() { return pipe::Detach{}; }

} // namespace futures
