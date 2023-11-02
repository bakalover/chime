#pragma once
namespace exec {
struct Runnable{
  virtual ~Runnable() = default;
  virtual void Run() noexcept = 0;
};
} // namespace exec
