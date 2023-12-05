#pragma once
#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <utility>

namespace executors {
template <typename Fun> void Submit(IExecutor &executor, Fun &&fun) {
  executor.Submit(executors::MakeContainer(std::move(fun)));
}
} // namespace executors