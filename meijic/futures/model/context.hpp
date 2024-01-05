#pragma once
#include <meijic/executors/executor.hpp>
#include <meijic/executors/impl/inline.hpp>
namespace futures {

struct Context {
  Context() : executor_{&executors::Inline()} {};
  executors::IExecutor *executor_;
};

} // namespace meijic::futures
