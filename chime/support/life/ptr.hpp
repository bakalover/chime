#pragma once
#include <chime/support/life/management.hpp>
#include <type_traits>
#include <utility>

namespace support {
template <typename T> class Ptr {
public:
  explicit Ptr(T *base) : base_{base} { base->ExtendLife(); }

  Ptr(Ptr &&) = default;
  Ptr &operator=(const Ptr &&) = default;

  template <typename U> Ptr(const Ptr<U> &other) : base_{other.base_} {
    static_assert(std::is_base_of_v<T, U>);
    base_->ExtendLife();
  }

  Ptr &operator=(const Ptr &other) {
    base_ = other.base_;
    base_->ExtendLife();
    return *this;
  }

  ~Ptr() { base_->ShortenLife(); }

private:
  LifeManagableBase<T> *base_;
};
template <typename T, typename... Args> Ptr<T> New(Args &&...args) {
  static_assert(std::is_base_of_v<LifeManagableBase<T>, T>,
                "Class in not life managable");
  return Ptr{new T(std::forward<Args>(args)...)};
}
} // namespace support