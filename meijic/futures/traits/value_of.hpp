#pragma once

#include <meijic/futures/types/future.hpp>

namespace futures::traits {

namespace match {

template <typename T>
struct ValueOf {};

template <SomeFuture F>
struct ValueOf<F> {
  using Type = typename F::ValueType;
};

}  // namespace match

template <typename Future>
using ValueOf = typename match::ValueOf<Future>::Type;

}  // namespace meijic::futures::traits
