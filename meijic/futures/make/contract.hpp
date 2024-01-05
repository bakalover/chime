#pragma once

#include <meijic/executors/impl/inline.hpp>
#include <meijic/futures/model/context.hpp>
#include <meijic/futures/model/output.hpp>
#include <meijic/futures/state/contract.hpp>
#include <meijic/futures/state/state.hpp>
#include <meijic/futures/types/eager.hpp>
#include <meijic/futures/types/future.hpp>
#include <meijic/result/make/err.hpp>
#include <meijic/result/make/ok.hpp>
#include <meijic/result/types/result.hpp>
#include <memory>
#include <tuple>
#include <utility>
namespace futures {


template <typename T> class Promise {
  using Ref = std::shared_ptr<state::Contract<T>>;

public:
  explicit Promise(Ref state_ref) : state_ref_{std::move(state_ref)} {}

  void Set(Output<T> output) && {
    ReleaseState()->SetOutput(std::move(output));
  }

  void SetValue(T value) && {
    std::move(*this).Set(MakeOutput(result::Ok(std::move(value))));
  }

  void Set(T value) && {
    std::move(*this).Set(MakeOutput(result::Ok(std::move(value))));
  }

  void SetError(result::Error error) && {
    std::move(*this).Set(MakeOutput(result::Err(std::move(error))));
  }

private:
  Ref ReleaseState() { return std::move(state_ref_); }

  Output<T> MakeOutput(result::Result<T> value) {
    return {std::move(value), Context{}};
  }

private:
  Ref state_ref_;
};

template <typename T> std::tuple<EagerFuture<T>, Promise<T>> Contract() {
  auto shared_state = std::make_shared<state::Contract<T>>();
  return {EagerFuture<T>{shared_state}, Promise<T>{shared_state}};
}

} // namespace futures
