#pragma once
#include <chime/futures/model/consumer.hpp>
#include <chime/result/make/err.hpp>
#include <chime/result/types/error.hpp>

namespace futures::thunks {

template <typename T> struct [[nodiscard]] Failure final {
  explicit Failure(result::Error with) : err_{with} {}

  using ValueType = T;

  Failure(const Failure &) = delete;

  Failure(Failure &&) = default;

  void Start(IConsumer<T> *consumer) { consumer->Complete(result::Err(err_)); }

  result::Error err_;
};
} // namespace futures::thunks