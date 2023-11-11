#pragma once

#include "meijic/executors/container.hpp"
#include "meijic/executors/executor.hpp"
#include <utility>

namespace exe {
template <typename Fun> void Submit(IExecutor &executor, Fun &&fun) {
  executor.Submit(exe::MakeContainer(std::move(fun)));
}
} // namespace exe