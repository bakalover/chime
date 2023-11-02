#pragma once
#include <meijic/executors/run.hpp>
#include <utility>
namespace exec {
template <typename Fun> struct Function : Runnable {
  Function(Fun fun) : fun_(fun) {}

  void Run() noexcept override { fun_(); }
  Function fun_;
};

template <typename Fun> Runnable *MakeFunTask(Fun fun) {
  return new Function(std::move(fun));
}
} // namespace exec
