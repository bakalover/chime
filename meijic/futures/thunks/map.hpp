#pragma once
#include <cassert>
#include <meijic/executors/task.hpp>
#include <meijic/futures/model/consumer.hpp>
#include <meijic/futures/model/context.hpp>
#include <meijic/futures/model/output.hpp>
#include <meijic/futures/traits/value_of.hpp>
#include <meijic/result/make/ok.hpp>
#include <meijic/result/traits/value_of.hpp>
#include <meijic/result/types/result.hpp>

namespace futures::thunks {

template <SomeFuture Producer, typename Mapper>
struct [[nodiscard]] Map final : private executors::TaskBase,
                                 private IConsumer<traits::ValueOf<Producer>> {
  using InputType = traits::ValueOf<Producer>;
  using OutputType = std::invoke_result_t<Mapper, result::Result<InputType>>;
  using ValueType = result::traits::ValueOf<OutputType>;

  // Start [trigger chain] -> Consume [recieve result by chain] ->
  // -> Run [apply myseft to executor] -> Consume next

public:
  explicit Map(Producer &&producer, Mapper &&mapper)
      : producer_{std::move(producer)}, mapper_{std::move(mapper)} {}

public:
  // Non-copyable
  Map(const Map &) = delete;

  // Movable
  Map(Map &&) = default;

  void Start(IConsumer<ValueType> *consumer) {
    consumer_ = consumer;
    producer_.Start(this);
  }

private:
  void Consume(Output<InputType> input) noexcept override {
    input_ = std::move(input);
    assert(input_.context.executor_ != nullptr);
    input_.context.executor_->Submit(this);
  }

  void Run() noexcept override {
    auto result = ActualMap();
    auto ctx = Context{};
    ctx.executor_ = input_.context.executor_;
    consumer_->Complete(Output<ValueType>{std::move(result), std::move(ctx)});
  }

  auto ActualMap() { return std::move(mapper_)(std::move(input_.result)); }

  IConsumer<ValueType> *consumer_;
  Output<InputType> input_;
  Producer producer_;
  Mapper mapper_;
};
} // namespace futures::thunks