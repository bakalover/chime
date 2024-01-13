#pragma once

#include <chime/executors/executor.hpp>
#include <chime/executors/impl/inline.hpp>

namespace futures {

struct Context {
  Context() : executor_{&executors::Inline()} {};
  executors::IExecutor *executor_;
};

} // namespace chime::futures
