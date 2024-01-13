#pragma once

#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/thunk.hpp>
#include <chime/futures/state/state.hpp>

namespace futures::thunks {

template <typename T> struct [[nodiscard]] Eager {
  using ValueType = T;

  explicit Eager(state::StateRef<T> ref) : ref_{std::move(ref)} {}

  // Non-copyable
  Eager(const Eager &) = delete;
  Eager &operator=(const Eager &) = delete;

  // Movable
  Eager(Eager &&) = default;
  Eager &operator=(Eager &&) = default;

  void Start(IConsumer<T> *consumer) { std::move(ref_)->SetConsumer(consumer); }

private:
  state::StateRef<T> ref_;
};

} // namespace futures::thunks
