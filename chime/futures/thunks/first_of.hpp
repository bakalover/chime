#pragma once
#include <chime/executors/impl/inline.hpp>
#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/pack/pack.hpp>
#include <chime/futures/thunks/box.hpp>
#include <chime/futures/thunks/index.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/make/err.hpp>
#include <chime/support/spinlock.hpp>
#include <cstdint>
#include <exception>
#include <optional>
#include <tuple>
#include <twist/ed/stdlike/atomic.hpp>
#include <utility>

namespace futures::thunks {

template <typename ParamPack>
struct [[nodiscard]] First final
    : private IConsumer<typename ParamPack::ValueType>,
      public internal::IBox<typename ParamPack::ValueType> {
  explicit First(ParamPack pack) : pack_{std::move(pack)} {}

  using ValueType = typename ParamPack::ValueType;

  // Non-copyable
  First(const First &) = delete;
  First &operator=(const First &) = delete;

  // Movable
  First(First &&other) : pack_{std::move(other.pack_)} {}
  First &operator=(First &&other) = default;

  void Start(IConsumer<ValueType> *consumer) override {
    consumer_ = consumer;
    this->ExtendLife(pack_.Size());
    pack_.Start(this);
  }

private:
  void Consume(Output<ValueType> output) noexcept override {
    {
      support::SpinLock::Guard guard{spin_};
      if (output.result.has_value()) {
        if (!state_.second) {
          state_.second = true;
          Complete(std::move(output));
        }
      } else {
        if (state_.first == pack_.Size() - 1) {
          Complete(std::move(output));
        }
      }
      state_.first++;
    }
    this->ShortenLife();
  }

  void Complete(Output<ValueType> output) {
    output.context.executor_ = &executors::Inline();
    consumer_->Complete(std::move(output));
  }

  ParamPack pack_;
  IConsumer<ValueType> *consumer_;
  std::pair<int64_t, bool> state_{0, false};
  support::SpinLock spin_;
};
} // namespace futures::thunks