#pragma once

#include <meijic/futures/combine/par/first.hpp>

template <futures::SomeFuture LeftFuture,
          futures::SomeFuture RightFuture>
auto operator||(LeftFuture f, RightFuture g) {
  return futures::First(std::move(f), std::move(g));
}
