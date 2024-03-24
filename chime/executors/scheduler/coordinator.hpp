#pragma once

#include "chime/executors/scheduler/worker.hpp"
#include "chime/support/spinlock.hpp"
#include "wheels/intrusive/forward_list.hpp"
#include <cstddef>
#include <cstdint>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/stdlike/mutex.hpp>

namespace executors::scheduler {

// Coordinates workers (stealing, parking)

static const uint64_t idle_shift_ = 32;
static const uint64_t idle_inc_ = (uint64_t)1 << idle_shift_;
static const uint64_t search_mask_ = ((uint64_t)1 << idle_shift_) - 1;
static const uint64_t idle_mask_ = !search_mask_;

class Coordinator {

public:
  explicit Coordinator(size_t threads) : threads_(threads) {}
  bool TrySpin();
  bool StopSpin();

  void BecomeIdle(Worker *worker);
  void BecomeActive();

  bool IsAllAsleep();
  void WakeOne();

private:
  uint64_t NumSearch();

private:
  support::SpinLock spinlock_;                        //  -> LockFree Intrusive
  wheels::IntrusiveForwardList<Worker> idle_workers_; //  -> LockFree Intrusive

  twist::ed::stdlike::atomic<std::uint64_t> state_{0};
  const size_t threads_;
};

} // namespace executors::scheduler
