#pragma once

#include <chime/futures/combine/seq/box.hpp>
#include <chime/futures/thunks/box.hpp>
#include <chime/futures/thunks/first_of.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
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
