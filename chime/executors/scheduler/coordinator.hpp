#pragma once

#include <cstddef>
#include <cstdint>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/stdlike/mutex.hpp>

namespace executors {

// Coordinates workers (stealing, parking)

class Coordinator {

public:
  explicit Coordinator(size_t threads) : threads_(threads) {}
  bool TrySpin();
  bool StopSpin();

private:
  uint64_t NumSearch();

private:
  twist::ed::stdlike::mutex mutex_;
  twist::ed::stdlike::atomic<std::uint64_t> state_{0};
  const size_t threads_;
  const uint64_t unpark_shift_ = 32;
  const uint64_t search_mask_ = (1 << unpark_shift_) - 1;
  const uint64_t unpark_mask_ = !search_mask_;
};

} // namespace executors
