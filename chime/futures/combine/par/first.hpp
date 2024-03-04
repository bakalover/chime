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
  auto pack = FormPack(std::forward<Producers>(prods)...);
  using GatherUnit = thunks::First<ParamPack<Producers...>>;
  auto first_ptr = support::New<GatherUnit>(std::move(pack));

  // On destroy of box first_ptr will be destroyed -> memory free
  return thunks::Boxed<traits::ValueOf<GatherUnit>>{first_ptr};
}

} // namespace futures
