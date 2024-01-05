#pragma once

#include <meijic/executors/executor.hpp>
#include <meijic/futures/combine/seq/map.hpp>
#include <meijic/futures/combine/seq/via.hpp>
#include <meijic/futures/make/just.hpp>
#include <meijic/futures/mappers/res_map.hpp>
#include <meijic/futures/syntax/pipe.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/traits/value_of.hpp>
#include <meijic/result/types/unit.hpp>
#include <type_traits>
#include <utility>

namespace futures {

template <typename F> auto Submit(executors::IExecutor &meijic, F fun) {
  return thunks::Map{Just() | Via(meijic),
                     mappers::ResultMapper<F>{std::move(fun)}};
}

} // namespace futures
