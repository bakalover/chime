#pragma once

#include <meijic/futures/types/naked.hpp>

#include <meijic/futures/thunks/eager.hpp>

namespace futures {

template <typename T> using EagerFuture = thunks::Eager<T>;

} // namespace futures
