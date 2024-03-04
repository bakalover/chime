#pragma once

#include <chime/futures/model/consumer.hpp>
#include <chime/futures/thunks/index.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <cstddef>
#include <utility>

namespace futures {
template <typename... Producers> class ParamPack {
  using Pack = std::tuple<Producers...>;

  // Should be the same
  using First = std::tuple_element_t<0, Pack>;

public:
  using ValueType = std::tuple<size_t, traits::ValueOf<First>>;

public:
  ParamPack(Producers &&...prods)
      : producers_{std::forward<Producers>(prods)...} {
    SetIndices();
  }

  // Only multi-triggerable consumer!!!
  void Start(IConsumer<ValueType> *consumer) {

    auto consume_set = [consumer](auto &&...prods) {
      (..., std::move(prods).Start(consumer));
    };

    std::apply(consume_set, producers_);
  }

  size_t Size() { return sizeof...(Producers); }

private:
  void SetIndices() {
    size_t index = 0;
    auto index_set = [&index](auto &&...prods) {
      (..., std::move(prods).SetIndex(index++));
    };
    std::apply(index_set, producers_);
  }

private:
  std::tuple<Producers...> producers_;
};

template <SomeFuture... Producers> auto FormPack(Producers &&...prods) {
  return ParamPack{thunks::Index{std::forward<Producers>(prods)}...};
}
} // namespace futures