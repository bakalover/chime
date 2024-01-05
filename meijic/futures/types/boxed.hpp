#pragma once

#include <meijic/futures/types/naked.hpp>

#include <meijic/futures/thunks/box.hpp>

namespace futures {

template <typename T>
using BoxedFuture = thunks::Boxed<T>;

}  // namespace futures
