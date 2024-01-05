#pragma once

#include <meijic/futures/mappers/just_map.hpp>
#include <meijic/futures/thunks/map.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <type_traits>
#include <utility>

namespace futures {

namespace pipe {

template <typename F> struct [[nodiscard]] Map {
  F fun;

  explicit Map(F f) : fun{std::move(f)} {}

  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return thunks::Map{
        std::move(input),
        mappers::JustMapper<traits::ValueOf<InputFuture>, F>{std::move(fun)}};
  }
};

} // namespace pipe

// Future<T> -> (T -> U) -> Future<U>

template <typename F> auto Map(F fun) { return pipe::Map{std::move(fun)}; }

} // namespace futures
