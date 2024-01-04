#pragma once

#include <meijic/result/types/result.hpp>
#include <meijic/result/types/status.hpp>

namespace meijic::result::make {

/*
 * Usage:
 *
 * futures::Value(1) | futures::AndThen([](int v) {
 *   return result::Ok(v + 1);
 * })
 *
 */

template <typename T> Result<T> Ok(T value) { return {std::move(value)}; }

/*
 * Usage:
 *
 * futures::Just() | futures::AndThen([](Unit) {
 *   return result::Ok();
 * });
 *
 */

inline Status Ok() { return {Unit{}}; }

} // namespace meijic::result::make
