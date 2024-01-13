#pragma once

#include <chime/result/make/ok.hpp>
#include <chime/result/traits/value_of.hpp>
#include <chime/result/types/result.hpp>
#include <chime/result/types/unit.hpp>
#include <type_traits>
#include <utility>

namespace futures::mappers {

template <typename F> class ResultMapper {
  F fun_;

public:
  explicit ResultMapper(F &&fun) : fun_{std::move(fun)} {}

  auto operator()(result::Result<result::Unit>) -> std::invoke_result_t<F> {
    return std::move(fun_)();
  }
};
} // namespace futures::mappers