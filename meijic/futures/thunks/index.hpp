#pragma once
#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/output.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/make/err.hpp>
#include <meijic/result/make/ok.hpp>
#include <tuple>

namespace futures::thunks {

template <SomeFuture Producer>
struct [[nodiscard]] Index final
    : private IConsumer<traits::ValueOf<Producer>> {
  explicit Index(size_t index, Producer &&producer)
      : index_{index}, producer_{std::move(producer)} {}

  using ProducerType = traits::ValueOf<Producer>;

  using ValueType = std::tuple<size_t, ProducerType>;

  Index(const Index &) = delete;

  Index(Index &&other) = default;

  void Start(IConsumer<ValueType> *consumer) {
    consumer_ = consumer;
    producer_.Start(this);
  }

private:
  void Consume(Output<ProducerType> output) noexcept override {
    if (output.result.has_value()) {
      consumer_->Complete(result::Ok(
          std::make_tuple(index_, std::move(output.result.value()))));
    } else {
      consumer_->Complete(result::Err(output.result.error()));
    }
  }

  size_t index_;
  Producer producer_;
  IConsumer<ValueType> *consumer_;
};
} // namespace futures::thunks