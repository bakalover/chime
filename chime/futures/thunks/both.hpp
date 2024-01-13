#pragma once
#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/thunks/box.hpp>
#include <chime/futures/thunks/index.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/make/err.hpp>
#include <chime/result/make/ok.hpp>
#include <chime/support/life/management.hpp>
#include <chime/support/spinlock.hpp>
#include <optional>
#include <tuple>
#include <utility>

namespace futures::thunks {

template <SomeFuture LeftProducer, SomeFuture RightProducer>
struct [[nodiscard]] Both final
    : private IConsumer<std::tuple<size_t, traits::ValueOf<LeftProducer>>>,
      public internal::IBox<std::tuple<traits::ValueOf<LeftProducer>,
                                       traits::ValueOf<RightProducer>>> {
  explicit Both(LeftProducer &&left, RightProducer &&right)
      : left_{Index<LeftProducer>{0, std::move(left)}},
        right_{Index<RightProducer>{1, std::move(right)}} {}

  using SingleIndexType = std::tuple<size_t, traits::ValueOf<LeftProducer>>;
  using SingleType = traits::ValueOf<LeftProducer>;
  using ValueType = std::tuple<SingleType, SingleType>;

  Both(const Both &) = delete;

  Both(Both &&other)
      : left_{std::move(other.left_)}, right_{std::move(other.right_)} {}

  void Start(IConsumer<ValueType> *consumer) override {
    consumer_ = consumer;
    this->ExtendLife();
    this->ExtendLife();
    left_.Start(this);
    right_.Start(this);
  }

private:
  void Consume(Output<SingleIndexType> output) noexcept override {
    {
      support::SpinLock::Guard guard{spinlock_};

      if (output.result.has_value()) {
        auto [i, val] = output.result.value();
        if (i == 0) {
          left_val_.emplace(std::move(val));
        } else {
          right_val_.emplace(std::move(val));
        }
        ++counter_;
        if (counter_ == 2) {
          RunToComplete();
        }
      } else {
        consumer_->Complete(result::Err(output.result.error()));
      }
    }
    this->ShortenLife();
  }

  void RunToComplete() {
    auto res = std::make_tuple(std::move(*left_val_), std::move(*right_val_));
    consumer_->Complete(result::Ok(std::move(res)));
  }

  Index<LeftProducer> left_;
  Index<RightProducer> right_;
  size_t counter_{0};
  std::optional<SingleType> left_val_, right_val_;
  support::SpinLock spinlock_;
  IConsumer<ValueType> *consumer_;
};
} // namespace futures::thunks