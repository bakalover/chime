#pragma once
#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/context.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/make/ok.hpp>

namespace futures::thunks {
template <SomeFuture Producer>
struct [[nodiscard]] Grave final
    : private IConsumer<traits::ValueOf<Producer>> {
public:
  explicit Grave(Producer &&thunk) : thunk_{std::move(thunk)} {}

  using ValueType = traits::ValueOf<Producer>;

  // Non-movable
  Grave(Grave &&) = delete;
  Grave &operator=(const Grave &&) = delete;

  // Non-copyable
  Grave(const Grave &) = delete;
  Grave &operator=(const Grave &) = delete;

  void BuryToGround() { thunk_.Start(this); }

private:
  void SelfDestruct() { delete this; }
  void Consume(Output<ValueType> /*no_need*/) noexcept override {
    SelfDestruct(); // Like non-returning Get()
  }

  Producer thunk_;
};
} // namespace futures::thunks