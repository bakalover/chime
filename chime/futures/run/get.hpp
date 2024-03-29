#pragma once

#include <cassert>
#include <chime/futures/model/consumer.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/syntax/pipe.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/result/types/result.hpp>
#include <chime/support/life/management.hpp>
#include <cstdint>
#include <optional>
#include <tl/expected.hpp>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/stdlike/condition_variable.hpp>
#include <twist/ed/stdlike/mutex.hpp>

namespace futures {

namespace pipe {

template <typename T>
class GetConsumer : public IConsumer<T>,
                    public support::LifeManagableBase<GetConsumer<T>> {
public:
  GetConsumer() { this->ExtendLife(); }

  void Consume(Output<T> output) noexcept override {
    {
      std::unique_lock<Mutex> ul{mutex_};
      result_.emplace(std::move(output.result));
      has_result_ = true;
    }
    is_ready_.notify_one();
    this->ShortenLife();
  }

  result::Result<T> GetResult() {
    result::Result<T> val;
    {
      std::unique_lock<Mutex> lk{mutex_};
      is_ready_.wait(lk, [this] { return HaveResult(); });
      val = std::move(result_.value());
    }
    this->ShortenLife();
    return std::move(val);
  }

private:
  bool HaveResult() { return has_result_; }

private:
  using Mutex = twist::ed::stdlike::mutex;

  Mutex mutex_;
  bool has_result_{false};
  twist::ed::stdlike::condition_variable is_ready_;
  std::optional<result::Result<T>> result_;
};

struct [[nodiscard]] Get {
  template <SomeFuture InputFuture>
  result::Result<traits::ValueOf<InputFuture>> Pipe(InputFuture input) {
    auto consumer = new GetConsumer<traits::ValueOf<InputFuture>>;
    input.Start(consumer);
    auto res = consumer->GetResult();
    return std::move(res);
  }
};

} // namespace pipe

inline auto Get() { return pipe::Get{}; }

} // namespace futures
