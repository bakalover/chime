#pragma once

#include <meijic/executors/executor.hpp>
#include <meijic/futures/thunks/via.hpp>
#include <meijic/futures/traits/value_of.hpp>

namespace futures {

namespace pipe {

struct [[nodiscard]] Via {
  executors::IExecutor &executor;

  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Via{std::move(input), &executor};
  }
};

} // namespace pipe

// Future<T> -> Executor -> Future<T>

inline auto Via(executors::IExecutor &executor) { return pipe::Via{executor}; }

} // namespace futures
