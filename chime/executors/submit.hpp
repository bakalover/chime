#pragma once
#include <chime/executors/container.hpp>
#include <chime/executors/executor.hpp>
#include <utility>

namespace executors {
template <typename Fun> void Submit(IExecutor &executor, Fun &&fun) {
  executor.Submit(executors::MakeContainer(std::move(fun)));
}
} // namespace executors