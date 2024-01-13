#pragma once

#include <tl/expected.hpp>

#include <chime/result/types/error.hpp>

namespace result {

// https://github.com/TartanLlama/expected

template <typename T> using Result = tl::expected<T, Error>;

} // namespace result
