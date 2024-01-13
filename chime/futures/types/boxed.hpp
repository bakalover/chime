#pragma once

#include <chime/futures/types/naked.hpp>

#include <chime/futures/thunks/box.hpp>

namespace futures {

template <typename T>
using BoxedFuture = thunks::Boxed<T>;

}  // namespace futures
