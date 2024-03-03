#pragma once

#include <chime/futures/model/consumer.hpp>
#include <chime/futures/thunks/index.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>

namespace futures {
template <typename... Producers> class ParamPack {
  using Pack = std::tuple<Producers...>;

  // Should be the same
  using First = std::tuple_element_t<0, Pack>;

public:
  using ValueType = traits::ValueOf<First>;

public:
  ParamPack(Producers &&...prods)
      : futures_{std::forward<Producers>(prods)...} {
    SetIndices();
  }

  // Only multi-triggerable consumer!!!
  void Start(IConsumer<ValueType> *consumer) {

    auto consume_set = [consumer](auto &&...prods) {
      (..., std::move(prods).Start(consumer));
    };

    std::apply(consume_set, futures_);
  }

  size_t Size() { return sizeof...(Producers); }

private:
  void SetIndices() {
    size_t index = 0;
    auto index_set = [&index](auto &&...prods) {
      (..., std::move(prods).SetIndex(index++));
    };
    std::apply(index_set, futures_);
  }

private:
  std::tuple<Producers...> futures_;
};

template <SomeFuture... Producers> auto FormPack(Producers &&...prods) {
  return FormPack(thunks::Index{std::forward<Producers>(prods)}...);
}
} // namespace futures