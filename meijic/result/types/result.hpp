#pragma once

#include <tl/expected.hpp>

#include <meijic/result/types/error.hpp>

namespace meijic::result {

// https://github.com/TartanLlama/expected

template <typename T> using Result = tl::expected<T, Error>;

} // namespace meijic::result::types
