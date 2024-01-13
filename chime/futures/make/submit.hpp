#pragma once

#include <chime/executors/executor.hpp>
#include <chime/futures/combine/seq/map.hpp>
#include <chime/futures/combine/seq/via.hpp>
#include <chime/futures/make/just.hpp>
#include <chime/futures/mappers/res_map.hpp>
#include <chime/futures/syntax/pipe.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/traits/value_of.hpp>
#include <chime/result/types/unit.hpp>
#include <type_traits>
#include <utility>

namespace futures {

template <typename F> auto Submit(executors::IExecutor &chime, F fun) {
  return thunks::Map{Just() | Via(chime),
                     mappers::ResultMapper<F>{std::move(fun)}};
}

} // namespace futures
