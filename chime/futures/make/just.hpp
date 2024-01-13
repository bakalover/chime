#pragma once

#include <chime/executors/impl/inline.hpp>
#include <chime/futures/thunks/value.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/make/ok.hpp>
#include <chime/result/types/unit.hpp>

namespace futures {

inline Future<result::Unit> auto Just() {
  return thunks::Value<result::Unit>{result::Unit{}};
}

} // namespace futures
