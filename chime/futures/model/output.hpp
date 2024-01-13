#pragma once
#include <chime/executors/executor.hpp>
#include <chime/result/types/result.hpp>
#include <chime/futures/model/context.hpp>

namespace futures {

template <typename T> struct Output {
  result::Result<T> result;
  Context context;
};

} // namespace chime::futures
