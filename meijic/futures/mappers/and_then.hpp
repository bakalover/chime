#include <asm-generic/errno.h>
#include <meijic/result/make/err.hpp>
#include <meijic/result/make/ok.hpp>
#include <meijic/result/types/error.hpp>
#include <meijic/result/types/result.hpp>
#include <type_traits>
#include <utility>
namespace futures::mappers {

template <typename T, typename F> class AndThenMapper {
  F fun_;

public:
  explicit AndThenMapper(F &&fun) : fun_{std::move(fun)} {}

  using U = std::invoke_result_t<F, T>;

  auto operator()(result::Result<T> result) -> U {
    if (result.has_value()) {
      auto res = fun_(std::move(result.value()));
      return std::move(res);
    } else {
      return result::Err(result.error());
    }
  }
};
} // namespace futures::mappers