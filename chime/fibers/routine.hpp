#pragma once

#include <utility>

namespace fibers {

struct IRoutine {
  virtual ~IRoutine() = default;

  virtual void Run() noexcept = 0;
};

template <typename Fun> struct RoutineContainer : IRoutine {

  // Non-copyable
  RoutineContainer(const RoutineContainer &) = delete;
  RoutineContainer &operator=(const RoutineContainer &) = delete;

  // Non-movable
  RoutineContainer(RoutineContainer &&that) = delete;
  RoutineContainer &operator=(RoutineContainer &&) = delete;

  explicit RoutineContainer(Fun &&fun) : lambda_(std::move(fun)) {}

  void Run() noexcept override {
    lambda_();
    SelfDestroy();
  }

  void SelfDestroy() { delete this; }

  Fun lambda_;
};

template <typename Fun> IRoutine *MakeRoutine(Fun &&fun) {
  return new RoutineContainer(std::move(fun));
}
} // namespace fibers