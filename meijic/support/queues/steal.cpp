#include <atomic>
#include <cstddef>
#include <meijic/support/queues/steal.hpp>
namespace sup::queue {

// Used by a single thread
template <typename T, size_t Cap> bool StealingQueue<T, Cap>::TryPush(T *item) {

  auto [curr_head, curr_tail] =
      QueueSnapshot(std::memory_order_acquire, std::memory_order_relaxed);

  if (IsFull(curr_head, curr_tail)) {
    return false;
  }

  buffer_[GetIndex(curr_tail)].item.store(item, std::memory_order_relaxed);
  tail_.store(curr_tail + 1, std::memory_order_release);
  return true;
}

// Used by a single thread
template <typename T, size_t Cap> T *StealingQueue<T, Cap>::TryPop() {
  while (true) {
    auto [curr_head, curr_tail] =
        QueueSnapshot(std::memory_order_acquire, std::memory_order_relaxed);

    if (IsEmpty(curr_head, curr_tail)) {
      return nullptr;
    }

    if (TryCommitHead(curr_head, 1)) {
      return buffer_[GetIndex(curr_head)].item.load(std::memory_order_relaxed);
    }
  }
}

// Used by multiple threads
template <typename T, size_t Cap>
size_t StealingQueue<T, Cap>::Grab(std::span<T *> out_buffer) {
  while (true) {
    auto [curr_head, curr_tail] =
        QueueSnapshot(std::memory_order_acquire, std::memory_order_acquire);

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
}
} // namespace supp::queue