#pragma once
#include <cstdlib>
#include <twist/ed/std/condition_variable.hpp>
#include <twist/ed/std/mutex.hpp>
namespace support {
// Group for wild threads
class Group {
public:
  void Add(size_t delta);
  void Done();
  void Wait();

private:
  using Mutex = twist::ed::std::mutex;
  Mutex mutex_;
  twist::ed::std::condition_variable cond_;
  size_t counter_{0}, sleeps_{0};
};
} // namespace support
