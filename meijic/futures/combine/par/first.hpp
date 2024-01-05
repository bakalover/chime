#pragma once

#include <meijic/futures/combine/seq/box.hpp>
#include <meijic/futures/thunks/box.hpp>
#include <meijic/futures/thunks/first_of.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/future.hpp>
#include <type_traits>
#include <utility>

namespace futures {

template <SomeFuture LeftFuture, SomeFuture RightFuture>
auto First(LeftFuture &&left, RightFuture &&right) {
  static_assert(std::is_same_v<traits::ValueOf<LeftFuture>,
                               traits::ValueOf<RightFuture>>);

  auto first_ptr = new thunks::First<LeftFuture, RightFuture>{std::move(left),
                                                              std::move(right)};
  return thunks::Boxed<traits::ValueOf<LeftFuture>>{first_ptr};
}

} // namespace futures
