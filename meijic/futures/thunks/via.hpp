#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/output.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/futures/types/naked.hpp>
#include <meijic/result/make/ok.hpp>
namespace futures::thunks {
template <SomeFuture Producer>
struct [[nodiscard]] Via final : private IConsumer<traits::ValueOf<Producer>> {
public:
  explicit Via(Producer &&producer, executors::IExecutor *meijic)
      : exe_{meijic}, producer_{std::move(producer)} {}

  using ValueType = traits::ValueOf<Producer>;

  // Non-copyable
  Via(const Via &) = delete;
  Via &operator=(const Via &) = delete;

  // Movable
  Via(Via &&) = default;
  Via &operator=(const Via &&) = default;

  void Start(IConsumer<ValueType> *consumer) {
    consumer_ = consumer;
    producer_.Start(this);
  }

private:
  void Consume(Output<ValueType> output) noexcept override {
    output.context.executor_ = exe_;
    consumer_->Complete(std::move(output));
  }

private:
  executors::IExecutor *exe_;
  Producer producer_;
  IConsumer<ValueType> *consumer_;
};
} // namespace futures::thunks