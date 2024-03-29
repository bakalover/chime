#pragma once
#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/make/err.hpp>
#include <chime/result/make/ok.hpp>
#include <cstddef>
#include <tuple>

namespace futures::thunks {

template <SomeFuture Producer>
struct [[nodiscard]] Index final
    : private IConsumer<traits::ValueOf<Producer>> {
  explicit Index(Producer &&producer) : producer_{std::move(producer)} {}

  using ProducerType = traits::ValueOf<Producer>;

  using ValueType = std::tuple<size_t, ProducerType>;

  Index(const Index &) = delete;

  Index(Index &&other) = default;

  void Start(IConsumer<ValueType> *consumer) {
    consumer_ = consumer;
    producer_.Start(this);
  }

  // Before Start()!!!!
  void SetIndex(size_t index) { index_ = index; }

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