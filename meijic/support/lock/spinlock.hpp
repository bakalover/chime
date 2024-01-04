#pragma once
#include <twist/ed/std/atomic.hpp>
namespace support {
class SpinLock {
public:
  class Guard {
    friend class SpinLock;

  public:
    explicit Guard(SpinLock &host);

    ~Guard();

  private:
    SpinLock &host_;
    twist::ed::std::atomic<Guard *> next_{nullptr};
    twist::ed::std::atomic<bool> is_owner_{false};
  };

  SpinLock(){};

private:
  void Acquire(Guard *waiter);
  void Release(Guard *owner);
  void CompleteLink(Guard *prev, Guard *next);
  void WaitForAccess(Guard *waiter);
  void WaitForLink(Guard *owner);
  void SignalToNext(Guard *owner);
  bool IsChainLeader(Guard *waiter);
  bool HaveNextWaiter(Guard *owner);

private:
  twist::ed::std::atomic<Guard *> tail_{nullptr};
};
} // namespace support
