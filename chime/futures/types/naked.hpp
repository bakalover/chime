#pragma once

#include <chime/futures/model/thunk.hpp>

namespace futures {

template <typename F>
concept SomeFuture = Thunk<F>;

template <typename F, typename T>
concept Future = SomeFuture<F> && std::same_as<typename F::ValueType, T>;

}  // namespace chime::futures
