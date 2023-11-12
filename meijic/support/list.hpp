#pragma once
#include <atomic>
#include <cassert>
#include <cstddef>
#include <functional>
#include <utility>
namespace sup {

struct ForwardListNodeDefaultTag {};

template <typename T, typename Tag = ForwardListNodeDefaultTag>
struct ForwardListNode {
  // Shortcut
  using Node = ForwardListNode;

  std::atomic<Node *> next_ = nullptr;

  Node *Next() noexcept { return next_; }

  void SetNext(Node *node) noexcept { next_ = node; }

  void ResetNext() noexcept { SetNext(nullptr); }

  void LinkBefore(Node *node) noexcept { SetNext(node); }

  T *AsItem() noexcept { return static_cast<T *>(this); }
};

template <typename T, typename Tag = ForwardListNodeDefaultTag>
class ForwardList {
  // Node type
  using Node = ForwardListNode<T, Tag>;

  using List = ForwardList<T, Tag>;

public:
  ForwardList() = default;

  // Non-copyable
  ForwardList(const List &) = delete;
  ForwardList &operator=(const List &) = delete;

  // Move-constructable
  ForwardList(List &&that) {
    state_ = that.state_;
    that.Reset();
  }

  ForwardList &operator=(List &&that) = delete;

  void PushBack(Node *node) noexcept {
    while (true) {

      Node *cur_head, curr_tail = state_.load(std::memory_order_acquire);

      node->next_.store(cur_head, std::memory_order_release);
    }
  }

  // O(1)
  //   void Append(List &that) noexcept {
  //     if (that.IsEmpty()) {
  //       return;
  //     }

  //     if (IsEmpty()) {
  //       head_ = that.head_;
  //       tail_ = that.tail_;
  //       size_ = that.size_;
  //     } else {
  //       tail_->next_ = that.head_;
  //       tail_ = that.tail_;
  //       size_ += that.size_;
  //     }

  //     that.Reset();
  //   }

  //   void PushFront(Node *node) noexcept {
  //     ++size_;
  //     if (IsEmpty()) {
  //       head_ = tail_ = node;
  //     } else {
  //       node->next_ = head_;
  //       head_ = node;
  //     }
  //   }

  //   T *PopFront() noexcept {
  //     if (IsEmpty()) {
  //       return nullptr;
  //     }

  //     --size_;
  //     Node *front = head_;

  //     if (head_ == tail_) {
  //       head_ = tail_ = nullptr;
  //     } else {
  //       head_ = head_->next_;
  //     }

  //     front->next_ = nullptr;

  //     return front->AsItem();
  //   }

  bool IsEmpty() const noexcept {
    return state_.load(std::memory_order_acquire).first == nullptr;
  }

  bool NonEmpty() const noexcept { return !IsEmpty(); }

  bool HasItems() const noexcept { return !IsEmpty(); }

private:
  void Reset() noexcept { state_.store(nullptr); }

private:
  // State.0 -> head
  // State.1 -> tail
  std::atomic<std::pair<Node *, Node *>> state_{nullptr, nullptr};
};
} // namespace sup