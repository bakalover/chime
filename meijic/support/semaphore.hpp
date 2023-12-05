#pragma once
#include <cassert>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
namespace support {
template <class Tag> class Semaphore {
public:
  class Token {
    friend class Semaphore;

  public:
    // Non-copyable
    Token(const Token &) = delete;
    Token &operator=(const Token &) = delete;

    // Movable
    Token(Token &&that) { that.Invalidate(); };
    Token &operator=(Token &&) = delete;

  private:
    ~Token() { assert(!valid_); };
    Token() = default;

    void Invalidate() {
      assert(valid_);
      valid_ = false;
    };

  private:
    bool valid_{true};
  };

public:
  explicit Semaphore(size_t tokens) : available_tokens_(tokens) {}

  Token Acquire() {
    std::unique_lock<std::mutex> guard(mutex_);
    while (available_tokens_ == 0) {
      cond_.wait(guard);
    }
    --available_tokens_;
    return Token{};
  };

  void Release(Token &&token) {
    {
      std::unique_lock<std::mutex> guard(mutex_);
      ++available_tokens_;
    }
    cond_.notify_one();
    token.Invalidate();
  };

private:
  size_t available_tokens_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

} // namespace sup
