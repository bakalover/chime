#pragma once
#include <chime/futures/model/consumer.hpp>
#include <memory>
namespace futures::state {

template <typename T> struct IState {
  virtual ~IState() = default;

  virtual void SetConsumer(IConsumer<T> *consumer) = 0;

  void Start(IConsumer<T> *consumer) { SetConsumer(consumer); }
};

//==========================================================

template <typename T> using StateRef = std::shared_ptr<IState<T>>;

} // namespace futures::state