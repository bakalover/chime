#pragma once
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <span>
#include <tuple>
namespace sup::queue {

template <typename T, size_t Cap> class StealingQueue {
  struct Slot {
    std::atomic<T *> item{nullptr};
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

  std::tuple<size_t, size_t> QueueSnapshot(std::memory_order head_order,
                                           std::memory_order tail_order) {
    return {head_.load(head_order), tail_.load(tail_order)};
  }

  void TransferData(std::span<T *> destination, size_t start_pos,
                    size_t amount) {
    for (size_t i = 0; i < amount; ++i) {
      destination[i] =
          buffer_[GetIndex(start_pos + i)].item.load(std::memory_order_acquire);
    }
  }

public:
  bool TryPush(T *item);

  // Returns nullptr if queue is empty
  // TODO: nullptr -> Option<T>
  T *TryPop();

  // Returns number of tasks
  size_t Grab(std::span<T *> out_buffer);

private:
  std::atomic<size_t> head_{0}, tail_{0};

  // Circle buffer
  std::array<Slot, Cap> buffer_;
};
} // namespace supp::queue
