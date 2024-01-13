#pragma once

#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/thunk.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/support/life/management.hpp>
#include <cstdlib>
#include <memory>
#include <utility>

namespace futures::thunks {

namespace internal {

// Fucking templates (bridge between <Thunk> and <T> parameterization)
template <typename T> struct IBox : support::LifeManagableBase<IBox<T>> {
  virtual ~IBox() = default;
  virtual void Start(IConsumer<T> *consumer) = 0;
};

template <typename T> using ThunkPtr = IBox<T> *;

//=========================================================

template <Thunk Thunk>
struct [[nodiscard]] Wrapper : IBox<traits::ValueOf<Thunk>> {
  using ValueType = traits::ValueOf<Thunk>;

  Wrapper(Thunk &&thunk) // NOLINT
      : thunk_{std::move(thunk)} {}

  // Non-copyable
  Wrapper(const Wrapper &) = delete;
  Wrapper &operator=(const Wrapper &) = delete;

  // Movable
  Wrapper(Wrapper &&) = default;
  Wrapper &operator=(Wrapper &&) = default;

  void Start(IConsumer<ValueType> *consumer) override {
    thunk_.Start(consumer);
  }

  Thunk thunk_;
};

//=========================================================

template <Thunk Thunk>
ThunkPtr<traits::ValueOf<Thunk>> WrapThunk(Thunk &&thunk) {
  return new Wrapper<Thunk>(std::move(thunk));
}

} // namespace internal

template <typename T> struct [[nodiscard]] Boxed final : private IConsumer<T> {
  using ValueType = T;

  explicit Boxed(internal::ThunkPtr<T> wrapped_ptr) : wrapped_{wrapped_ptr} {}

  // Auto-boxing
  template <Thunk Thunk>
  Boxed(Thunk &&thunk) // NOLINT
      : wrapped_{internal::WrapThunk<Thunk>(std::move(thunk))} {}

  // Non-copyable
  Boxed(const Boxed &) = delete;
  Boxed &operator=(const Boxed &) = delete;

  // Movable
  Boxed(Boxed &&) = default;
  Boxed &operator=(Boxed &&other) = default;

  void Start(IConsumer<T> *consumer) {
    consumer_ = consumer;
    wrapped_->Start(this);
  }

private:
  void Consume(Output<ValueType> output) noexcept override {
    wrapped_->ShortenLife();
    consumer_->Complete(std::move(output));
  }

  IConsumer<T> *consumer_;
  internal::ThunkPtr<T> wrapped_;
};
//========================================================

} // namespace futures::thunks
