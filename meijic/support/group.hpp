#pragma once
#include <condition_variable>
#include <cstdlib>
#include <mutex>
namespace sup {
// Group for wild threads
class Group {
public:
  void Add(size_t delta);
  void Done();
  void Wait();

private:
  std::mutex mutex_;
  std::condition_variable cond_;
  size_t counter_{0}, sleeps_{0};
};
} // namespace sup
