#pragma once

#include <chime/futures/types/future.hpp>

#include <chime/futures/combine/seq/start.hpp>

/*
 * "Bang" operator (!)
 *
 * Named after bang patterns in Strict Haskell
 * https://www.fpcomplete.com/haskell/tutorial/all-about-strictness/
 *
 */

template <futures::SomeFuture InputFuture> auto operator!(InputFuture f) {
  return std::move(f) | futures::Bang();
}
