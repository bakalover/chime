#pragma once
namespace exec {
struct Task {
  virtual ~Task() = default;
  virtual void Run() noexcept = 0;
};
} // namespace exec
