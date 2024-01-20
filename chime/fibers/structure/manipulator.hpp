#pragma once

#include <chime/fibers/handle.hpp>

namespace fibers {

struct IManipulator {
  virtual ~IManipulator() = default;
  virtual void Manipulate(fibers::FiberHandle slave) = 0;
};

} // namespace fibers::structure