#pragma once

#include <meijic/executors/executor.hpp>
#include <meijic/executors/funtask.hpp>
#include <utility>
namespace exec {
template <typename Fun> void Submit(Executor &executor, Fun &&fun) {
  executor.Submit(exec::MakeFunTask(std::move((fun))));
}
} // namespace exec