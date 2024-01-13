#pragma once

#include <chime/futures/mappers/or_else.hpp>
#include <chime/futures/thunks/map.hpp>
#include <chime/futures/traits/value_of.hpp>

namespace futures {

namespace pipe {

template <typename F> struct [[nodiscard]] OrElse {
  F fun;

  explicit OrElse(F f) : fun{std::move(f)} {}

  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Map{
        std::move(input),
        mappers::OrElseMapper<traits::ValueOf<InputFuture>, F>{std::move(fun)}};
  }
};

} // namespace pipe

template <typename F> auto OrElse(F fun) {
  return pipe::OrElse{std::move(fun)};
}

} // namespace futures
