#pragma once

#include <chime/executors/executor.hpp>
#include <chime/futures/thunks/via.hpp>
#include <chime/futures/traits/value_of.hpp>

namespace futures {

namespace pipe {

struct [[nodiscard]] Via {
  executors::IExecutor &executor;

  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Via{std::move(input), &executor};
  }
};

} // namespace pipe


inline auto Via(executors::IExecutor &executor) { return pipe::Via{executor}; }

} // namespace futures
