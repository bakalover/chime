#pragma once
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <span>
#include <tuple>
#include <twist/ed/stdlike/atomic.hpp>
namespace support::queue {

template <typename T, size_t Cap> class StealingQueue {
  struct Slot {
    twist::ed::stdlike::atomic<T *> item{nullptr};
  };

private:
  size_t GetIndex(size_t pos) { return pos % Cap; }
  bool IsFull(size_t head, size_t tail) { return tail - head >= Cap; }
  bool IsEmpty(size_t head, size_t tail) { return head == tail; }

  bool TryCommitHead(size_t curr_head, size_t delta) {
    return head_.compare_exchange_weak(curr_head, curr_head + delta,
                                       std::memory_order_release);
  }

  size_t ActualGrabSize(std::span<T *> out_buffer, size_t having) {
    return (out_buffer.size() < having) ? out_buffer.size() : having;
  }

  void TransferData(std::span<T *> destination, size_t start_pos,
                    size_t amount) {
    for (size_t i = 0; i < amount; ++i) {
      destination[i] =
          buffer_[GetIndex(start_pos + i)].item.load(std::memory_order_acquire);
    }
  }

public:
  size_t Size() { return tail_.load() - head_.load(); } // Orderings !!

  bool TryPush(T *item) {
    size_t curr_head = head_.load(std::memory_order_acquire);
    size_t curr_tail = tail_.load(std::memory_order_relaxed);

    if (IsFull(curr_head, curr_tail)) {
      return false;
    }

    buffer_[GetIndex(curr_tail)].item.store(item, std::memory_order_relaxed);
    tail_.store(curr_tail + 1, std::memory_order_release);
    return true;
  };

  void PushMany(std::span<T *> buffer) {
    size_t curr_head = head_.load(); // Ordering !!
    size_t curr_tail = tail_.load(); // Ordering !!

    for (size_t i = 0; i < buffer.size(); ++i) {
      buffer[GetIndex(curr_tail + i)].item.store(buffer[i]); // Ordering !!
    }

    tail_.store(curr_tail + buffer.size(), std::memory_order::release);
  }

  // Returns nullptr if queue is empty
  // TODO: nullptr -> Option<T>
  T *TryPop() {
    while (true) {
      size_t curr_head = head_.load(std::memory_order_acquire);
      size_t curr_tail = tail_.load(std::memory_order_relaxed);

      if (IsEmpty(curr_head, curr_tail)) {
        return nullptr;
      }

      if (TryCommitHead(curr_head, 1)) {
        return buffer_[GetIndex(curr_head)].item.load(
            std::memory_order_relaxed);
      }
    }
  };

  // Returns number of tasks
  size_t Grab(std::span<T *> out_buffer) {
    while (true) {
      size_t curr_head = head_.load(std::memory_order_acquire);
      size_t curr_tail = tail_.load(std::memory_order_acquire);

      size_t size = curr_tail - curr_head;

      if (IsEmpty(curr_head, curr_tail)) {
        return 0;
      }

      size_t to_grab = ActualGrabSize(out_buffer, size);

      TransferData(out_buffer, curr_head, to_grab);

      if (TryCommitHead(curr_head, to_grab)) {
        return to_grab;
      }
    }
  };

private:
  twist::ed::stdlike::atomic<size_t> head_{0}, tail_{0};

  // Circle buffer
  std::array<Slot, Cap> buffer_;
};
} // namespace support::queue
