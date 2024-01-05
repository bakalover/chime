#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/output.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/make/err.hpp>
#include <optional>

namespace futures::thunks {

template <SomeFuture Producer>
struct [[nodiscard]] Flatten final
    : private IConsumer<traits::ValueOf<Producer>>,
      private IConsumer<traits::ValueOf<traits::ValueOf<Producer>>> {
  Flatten(Producer outer_producer) // NOLINT
      : outer_producer_{std::move(outer_producer)} {}

  using InnerType = traits::ValueOf<traits::ValueOf<Producer>>;
  using OuterType = traits::ValueOf<Producer>;
  using ValueType = InnerType;

  Flatten(const Flatten &) = delete;

  Flatten(Flatten &&) = default;

  void Start(IConsumer<InnerType> *consumer) {
    consumer_ = consumer;
    outer_producer_.Start(this);
  }

private:
  void Consume(Output<OuterType> output) noexcept override {
    outer_executor_ = output.context.executor_;
    if (output.result.has_value()) {
      inner_producer_.emplace(std::move(output.result.value()));
      std::move(*inner_producer_).Start(this);
    } else {
      consumer_->Complete(result::Err(std::move(output.result.error())));
    }
  }

  void Consume(Output<InnerType> output) noexcept override {
    output.context.executor_ = outer_executor_;
    consumer_->Complete(std::move(output));
  }

  Producer outer_producer_;
  executors::IExecutor *outer_executor_{nullptr};
  std::optional<OuterType> inner_producer_;
  IConsumer<InnerType> *consumer_;
};
} // namespace futures::thunks