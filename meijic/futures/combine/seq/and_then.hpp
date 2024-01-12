#pragma once

#include <meijic/futures/mappers/and_then.hpp>
#include <meijic/futures/thunks/map.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/result/traits/value_of.hpp>
#include <type_traits>

namespace futures {

namespace pipe {

template <typename F> struct [[nodiscard]] AndThen {
  F fun;

  explicit AndThen(F f) : fun{std::move(f)} {}

  // Non-copyable
  AndThen(AndThen &) = delete;

  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Map{std::move(input),
                       mappers::AndThenMapper<traits::ValueOf<InputFuture>, F>{
                           std::move(fun)}};
  }
};

} // namespace pipe

template <typename F> auto AndThen(F fun) {
  return pipe::AndThen{std::move(fun)};
}

} // namespace futures
