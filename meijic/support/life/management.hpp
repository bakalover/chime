#pragma once
#include <atomic>
#include <cstddef>
#include <twist/ed/std/atomic.hpp>

namespace support::life {
template <typename T> struct LifeManagableBase {
  void ShortenLife() {
    if (life_counter.fetch_sub(1, std::memory_order::acq_rel) == 1) {
      SelfDestroy();
    }
  }

  void ExtendLife() { life_counter.fetch_add(1, std::memory_order::release); }

  void SelfDestroy() { delete static_cast<T *>(this); }

  twist::ed::std::atomic<size_t> life_counter{1};
};
} // namespace support::life