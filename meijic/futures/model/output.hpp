#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/result/types/result.hpp>
#include <meijic/futures/model/context.hpp>

namespace futures {

template <typename T> struct Output {
  result::Result<T> result;
  Context context;
};

} // namespace meijic::futures
