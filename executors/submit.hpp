#pragma once

#include <meijic/executors/container.hpp>
#include <meijic/executors/executor.hpp>
#include <utility>
namespace exec {
// Proceed standalone task
template <typename Fun> void Submit(IExecutor &executor, Fun &&fun) {
  executor.Submit(exec::MakeContainer(std::move(fun)));
}
} // namespace exec