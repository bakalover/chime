#pragma once
#include <cassert>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
namespace sup {
template <class Tag> class Semaphore {
public:
  class Token {
    friend class Semaphore;

  public:
    ~Token();

    // Non-copyable
    Token(const Token &) = delete;
    Token &operator=(const Token &) = delete;

    // Movable
    Token(Token &&that);
    Token &operator=(Token &&) = delete;

  private:
    Token() = default;

    void Invalidate();

  private:
    bool valid_{true};
  };

public:
  explicit Semaphore(size_t tokens) : available_tokens_(tokens) {}

  Token Acquire();

  void Release(Token &&token);

private:
  size_t available_tokens_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

} // namespace supp
