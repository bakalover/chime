#pragma once

#include <chime/futures/mappers/just_map.hpp>
#include <chime/futures/thunks/map.hpp>
#include <chime/futures/traits/value_of.hpp>
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

template <typename F> auto Map(F fun) { return pipe::Map{std::move(fun)}; }

} // namespace futures
