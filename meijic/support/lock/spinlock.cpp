#include <meijic/support/lock/spinlock.hpp>
namespace support {
void SpinLock::Acquire(SpinLock::Guard *waiter) {
  auto prev_tail = tail_.exchange(waiter);
  if (IsChainLeader(prev_tail)) {
    return;
  }
  CompleteLink(prev_tail, waiter);
  WaitForAccess(waiter);
}

void SpinLock::Release(SpinLock::Guard *owner) {
  if (HaveNextWaiter(owner)) {
    WaitForLink(owner);
    SignalToNext(owner);
  }
}

void SpinLock::WaitForAccess(SpinLock::Guard *waiter) {
  while (!waiter->is_owner_.load(std::memory_order_acquire)) {
  }
}

void SpinLock::CompleteLink(SpinLock::Guard *prev, SpinLock::Guard *next) {
  prev->next_.store(next, std::memory_order_release);
}

bool SpinLock::IsChainLeader(SpinLock::Guard *prev) { return prev == nullptr; }

bool SpinLock::HaveNextWaiter(SpinLock::Guard *owner) {
  return !tail_.compare_exchange_strong(owner, nullptr);
}

void SpinLock::WaitForLink(SpinLock::Guard *owner) {
  while (owner->next_.load(std::memory_order_acquire) == nullptr) {
  }
}

void SpinLock::SignalToNext(SpinLock::Guard *owner) {
  owner->next_.load(std::memory_order_acquire)
      ->is_owner_.store(true, std::memory_order_release);
}

SpinLock::Guard::Guard(SpinLock &host) : host_(host) { host_.Acquire(this); }

SpinLock::Guard::~Guard() { host_.Release(this); }
} // namespace support