#pragma once
#include <chime/executors/tasks/container.hpp>
#include <chime/executors/tasks/executor.hpp>
#include <utility>

namespace executors {
template <typename Fun> void Submit(IExecutor &executor, Fun &&fun) {
  executor.Submit(executors::MakeContainer(std::move(fun)));
}
} // namespace executors