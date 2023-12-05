#pragma once
#include <meijic/executors/task.hpp>
#include <utility>

namespace executors {

template <typename Fun> struct LambdaContainer : TaskBase {

  // Non-copyable
  LambdaContainer(const LambdaContainer &) = delete;
  LambdaContainer &operator=(const LambdaContainer &) = delete;

  // Non-movable
  LambdaContainer(LambdaContainer &&that) = delete;
  LambdaContainer &operator=(LambdaContainer &&) = delete;

  explicit LambdaContainer(Fun&& fun) : lambda_(fun) {}

  void Run() noexcept override {
    lambda_();
    delete this;
  }
  Fun lambda_;
};

// TODO get rid of TaskBase, also separate in executors
template <typename Fun> TaskBase *MakeContainer(Fun&& fun) {
  return new LambdaContainer(std::move(fun));
}
} // namespace executors
