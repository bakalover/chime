#pragma once
#include <meijic/executors/task.hpp>
#include <utility>
namespace exec {
template <typename Fun> struct FunTask : Task {
  FunTask(Fun fun) : fun_(fun) {}

  void Run() noexcept override { fun_(); }
  Fun fun_;
};

template <typename Fun> Task *MakeFunTask(Fun fun) {
  return new FunTask(std::move(fun));
}
} // namespace exec
