#pragma once

#include <chime/futures/combine/seq/box.hpp>
#include <chime/futures/pack/pack.hpp>
#include <chime/futures/thunks/box.hpp>
#include <chime/futures/thunks/first_of.hpp>
#include <chime/futures/traits/value_of.hpp>
#include <chime/futures/types/future.hpp>
#include <chime/support/life/ptr.hpp>
#include <type_traits>
#include <utility>

namespace futures {

template <SomeFuture... Producers> auto First(Producers &&...prods) {
  auto args = FormPack(std::forward<Producers>(prods)...);
  auto first_ptr = support::New<thunks::First>(std::move(args));
  using GatherUnit = thunks::First<Producers...>;
  return thunks::Boxed<traits::ValueOf<GatherUnit>>{first_ptr};
}

} // namespace futures
