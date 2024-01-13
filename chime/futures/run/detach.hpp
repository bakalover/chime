#pragma once

#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/syntax/pipe.hpp>
#include <chime/futures/thunks/grave.hpp>
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
