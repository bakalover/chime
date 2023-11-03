#pragma once
#include <meijic/executors/task.hpp>
#include <utility>
namespace exec {

// Standalone task without linking
template <typename Fun> struct LambdaContainer : ITask {

  // Non-copyable
  LambdaContainer(const LambdaContainer &) = delete;
  LambdaContainer &operator=(const LambdaContainer &) = delete;

  // Non-movable
  LambdaContainer(LambdaContainer &&that) = delete;
  LambdaContainer &operator=(LambdaContainer &&) = delete;

  LambdaContainer(Fun fun) : lambda_(fun) {}

  void Run() noexcept override {
    lambda_();
    delete this;
  }
  LambdaContainer lambda_;
};

template <typename Fun> ITask *MakeContainer(Fun fun) {
  return new LambdaContainer(std::move(fun));
}
} // namespace exec
