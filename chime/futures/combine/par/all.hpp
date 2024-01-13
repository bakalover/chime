#pragma once

#include <chime/futures/combine/seq/box.hpp>
#include <chime/futures/thunks/both.hpp>
#include <chime/futures/thunks/box.hpp>
#include <chime/futures/thunks/index.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <tuple>
#include <utility>

namespace futures {

template <SomeFuture LeftFuture, SomeFuture RightFuture>
auto Both(LeftFuture &&left, RightFuture &&right) {
  using BothType =
      std::tuple<traits::ValueOf<LeftFuture>, traits::ValueOf<RightFuture>>;

  auto all = new thunks::Both{std::move(left), std::move(right)};
  return thunks::Boxed<BothType>{all};
}

// All ???

} // namespace futures
