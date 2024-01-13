#pragma once

#include <chime/fibers/handle.hpp>

namespace fibers::structure {

struct IManipulator {
  virtual ~IManipulator() = default;
  virtual void Manipulate(fibers::FiberHandle *task) = 0;
};

} // namespace fibers::structure