#pragma once
#include <meijic/result/types/result.hpp>

namespace meijic::result::make {

/*
 *
 * Usage:
 *
 * futures::Just() | futures::AndThen([](Unit) -> Result<int> {
 *   return result::Err(Timeout());
 * });
 *
 */

inline auto Err(Error error) { return tl::unexpected(error); }

} // namespace meijic::result::make
