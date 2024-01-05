#pragma once

#include <meijic/futures/types/naked.hpp>

template <futures::SomeFuture Future, typename C>
auto operator|(Future &&f, C c) {
  return c.Pipe(std::move(f));
}
