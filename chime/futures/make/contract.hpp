#pragma once

#include <chime/executors/impl/inline.hpp>
#include <chime/futures/model/context.hpp>
#include <chime/futures/model/output.hpp>
#include <chime/futures/state/contract.hpp>
#include <chime/futures/state/state.hpp>
#include <chime/futures/types/eager.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/result/make/err.hpp>
#include <chime/result/make/ok.hpp>
#include <chime/result/types/result.hpp>
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
