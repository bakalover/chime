#pragma once

#include <chime/futures/types/naked.hpp>

#include <chime/futures/thunks/eager.hpp>

namespace futures {

template <typename T> using EagerFuture = thunks::Eager<T>;

} // namespace futures
