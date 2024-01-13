#pragma once

#include <asm-generic/errno.h>
#include <chime/result/make/ok.hpp>
#include <chime/result/types/result.hpp>
#include <utility>

namespace futures::mappers {

template <typename T, typename F> class OrElseMapper {
  F fun_;

public:
  explicit OrElseMapper(F &&fun) : fun_{std::move(fun)} {}

  using U = std::invoke_result_t<F, result::Error>;

  auto operator()(result::Result<T> result) -> U {
    if (!result.has_value()) {
      auto res = fun_(std::move(result.error()));
      return std::move(res);
    } else {
      return std::move(result);
    }
  }
};
} // namespace futures::mappers