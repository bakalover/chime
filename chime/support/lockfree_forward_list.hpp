#pragma once
#include <atomic>
#include <twist/ed/stdlike/atomic.hpp>


namespace support {
template <typename T>
struct LockFreeForwardListNode {
    using Node = LockFreeForwardListNode;

    Node *next_ = nullptr;

    Node *Next() noexcept {
        return next_;
    }

    void SetNext(Node *node) noexcept {
        next_ = node;
    }

    void ResetNext() noexcept {
        SetNext(nullptr);
    }

    T *AsItem() noexcept {
        return static_cast<T *>(this);
    }
};

template <typename T>
class LockFreeForwardList {
    using Node = LockFreeForwardListNode<T>;

  public:
    LockFreeForwardList() = default;

    // Non-copyable
    LockFreeForwardList(const LockFreeForwardList &) = delete;
    LockFreeForwardList &operator=(const LockFreeForwardList &) = delete;

    // Move-constructable
    LockFreeForwardList(LockFreeForwardList &&that) {
        head_ = that.head_;
        tail_ = that.tail_;
        size_ = that.size_;
        that.Reset();
    }

    LockFreeForwardList &operator=(LockFreeForwardList &&that) = delete;

    void PushBack(Node *node) noexcept {
        while (true) {
            Node *curr_tail = tail_.load();
            if (tail_.compare_exchange_weak(curr_tail, node)) {
                if (IsEmpty()) {
                    head_.store(tail_.load());
                    break;
                } else {
                    curr_tail->next.store(node);
                    break;
                }
            }
        }
        ++size_;

        node->next_ = nullptr;

        if (IsEmpty()) {
            head_ = tail_ = node;
        } else {
            tail_->next_ = node;
            tail_ = node;
        }
    }

    void PushBack(const T &value) {
        Node<T> *newNode = new Node<T>(value);
        Node<T> *nullPtr = nullptr;
        while (true) {
            Node<T> *last = tail.load();
            if (tail.compare_exchange_weak(last, newNode)) {
                if (last) {
                    last->next.store(newNode);
                    break;
                } else {
                    head.store(newNode);
                    break;
                }
            }
        }
    }

    // O(1)
    void Append(LockFreeForwardList<T> &that) noexcept {
        if (IsEmpty()) {
            head_ = that.head_;
            tail_ = that.tail_;
            size_ = that.size_;
        } else {
            tail_->next_ = that.head_;
            tail_ = that.tail_;
            size_ += that.size_;
        }

        that.Reset();
    }

    void PushFront(Node *node) noexcept {
        ++size_;
        if (IsEmpty()) {
            head_ = tail_ = node;
        } else {
            node->next_ = head_;
            head_ = node;
        }
    }

    T *PopFront() noexcept {
        if (IsEmpty()) {
            return nullptr;
        }

        --size_;
        Node *front = head_;

        if (head_ == tail_) {
            head_ = tail_ = nullptr;
        } else {
            head_ = head_->next_;
        }

        front->next_ = nullptr;

        return front->AsItem();
    }

    bool IsEmpty() const noexcept {
        return head_.load(std::memory_order::acquire) == nullptr;
    }

    bool NonEmpty() const noexcept {
        return !IsEmpty();
    }

    bool HasItems() const noexcept {
        return !IsEmpty();
    }

    // O(1) worst case
    size_t Size() const noexcept {
        return size_;
    }

    // TODO: Iterators

    template <typename F>
    void ForEach(F &&handler) {
        Node *iter = head_;
        while (iter != nullptr) {
            handler(iter->AsItem());
            iter = iter->next_;
        }
    }

    ~LockFreeForwardList() {
        WHEELS_ASSERT(IsEmpty(), "List is not empty");
    }

  private:
    void Reset() noexcept {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

  private:
    twist::ed::stdlike::atomic<Node *> head_{nullptr};
    twist::ed::stdlike::atomic<Node *> tail_{nullptr};
    twist::ed::stdlike::atomic<size_t> size_{0};
};
}