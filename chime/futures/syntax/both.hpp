#pragma once

#include <chime/futures/combine/par/all.hpp>

template <futures::SomeFuture LeftFuture, futures::SomeFuture RightFuture>
auto operator+(LeftFuture f, RightFuture g) {
  return futures::Both(std::move(f), std::move(g));
}
