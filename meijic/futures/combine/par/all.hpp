#pragma once
#include <meijic/futures/combine/seq/box.hpp>
#include <meijic/futures/thunks/both.hpp>
#include <meijic/futures/thunks/box.hpp>
#include <meijic/futures/thunks/index.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/future.hpp>
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
