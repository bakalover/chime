#pragma once
#include <cstdint>
#include <exception>
#include <chime/executors/impl/inline.hpp>
#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/thunks/box.hpp>
#include <chime/futures/thunks/index.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/make/err.hpp>
#include <chime/support/spinlock.hpp>
#include <optional>
#include <twist/ed/std/atomic.hpp>

namespace futures::thunks {

template <SomeFuture LeftProducer, SomeFuture RightProducer>
struct [[nodiscard]] First final
    : private IConsumer<traits::ValueOf<LeftProducer>>,
      public internal::IBox<traits::ValueOf<LeftProducer>> {
  explicit First(LeftProducer &&left, RightProducer &&right)
      : left_{std::move(left)}, right_{std::move(right)} {}

  using ValueType = traits::ValueOf<LeftProducer>;

  // Non-copyable
  First(const First &) = delete;
  First &operator=(const First &) = delete;

  // Movable
  First(First &&other)
      : left_(std::move(other.left_)), right_(std::move(other.right_)) {}
  First &operator=(First &&other) = default;

  void Start(IConsumer<ValueType> *consumer) override {
    consumer_ = consumer;
    this->ExtendLife();
    this->ExtendLife();
    left_.Start(this);
    right_.Start(this);
  }

private:
  void Consume(Output<ValueType> output) noexcept override {
    {
      support::SpinLock::Guard guard{spin_};
      if (output.result.has_value()) {
        if (state_ % 2 == 0) {
          Complete(std::move(output));
        }
        ++state_;
      } else {
        if (state_ == 2) { // 2 + 2 = 4 aka last Error
          Complete(std::move(output));
        }
        state_ += 2;
      }
    }
    this->ShortenLife();
  }

  void Complete(Output<ValueType> output) {
    output.context.executor_ = &executors::Inline();
    consumer_->Complete(std::move(output));
  }

  LeftProducer left_;
  RightProducer right_;
  IConsumer<ValueType> *consumer_;
  uint8_t state_{0};
  support::SpinLock spin_;
};
} // namespace futures::thunks