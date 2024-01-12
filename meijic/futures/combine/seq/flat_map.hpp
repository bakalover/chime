#pragma once

#include <meijic/futures/combine/seq/flatten.hpp>
#include <meijic/futures/combine/seq/map.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <type_traits>
#include <utility>

namespace futures {

namespace pipe {

template <typename F> struct [[nodiscard]] FlatMap {
  F fun;

  explicit FlatMap(F f) : fun(std::move(f)) {}

  template <SomeFuture InputFuture> auto Pipe(InputFuture input) {
    return std::move(input) | futures::Map(std::move(fun)) | futures::Flatten();
  }
};

} // namespace pipe

template <typename F> auto FlatMap(F fun) {
  return pipe::FlatMap{std::move(fun)};
}

} // namespace futures
