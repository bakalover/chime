#pragma once

#include <meijic/executors/impl/inline.hpp>
#include <meijic/futures/thunks/value.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/make/ok.hpp>
#include <meijic/result/types/unit.hpp>

namespace futures {

inline Future<result::Unit> auto Just() {
  return thunks::Value<result::Unit>{result::Unit{}};
}

} // namespace futures
