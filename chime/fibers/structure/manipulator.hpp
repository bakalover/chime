#pragma once

#include <chime/fibers/handle.hpp>

namespace fibers::structure {

struct IManipulator {
  virtual ~IManipulator() = default;
  virtual void Manipulate(fibers::FiberHandle slave) = 0;
};

} // namespace fibers::structure