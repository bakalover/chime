#pragma once
#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/context.hpp>
#include <meijic/result/make/ok.hpp>

namespace futures::thunks {
template <typename T> struct [[nodiscard]] Value final {
public:
  explicit Value(T value) : value_{std::move(value)} {}

  using ValueType = T;

  // Movable
  Value(Value &&other) : value_(std::move(other.value_)) {}
  Value &operator=(const Value &&) = default;

  // Non-copyable
  Value(const Value &) = delete;
  Value &operator=(const Value &) = delete;

  void Start(IConsumer<T> *consumer) {
    consumer->Complete(result::Ok(std::move(value_)));
  }

  T value_;
};
} // namespace futures::thunks