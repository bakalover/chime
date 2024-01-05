#include <meijic/result/make/ok.hpp>
#include <meijic/result/traits/value_of.hpp>
#include <meijic/result/types/result.hpp>
#include <meijic/result/types/unit.hpp>
#include <type_traits>
#include <utility>
namespace futures::mappers {

template <typename F> class ResultMapper {
  F fun_;

public:
  explicit ResultMapper(F &&fun) : fun_{std::move(fun)} {}

  auto operator()(result::Result<result::Unit>) -> std::invoke_result_t<F> {
    return std::move(fun_)();
  }
};
} // namespace futures::mappers