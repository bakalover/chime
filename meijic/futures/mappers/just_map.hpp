#include <meijic/result/make/err.hpp>
#include <meijic/result/make/ok.hpp>
#include <meijic/result/types/error.hpp>
#include <meijic/result/types/result.hpp>
#include <utility>
namespace futures::mappers {

  template <typename T, typename F> class JustMapper {
    F fun_;

  public:
    explicit JustMapper(F &&fun) : fun_{std::move(fun)} {}

    using U = std::invoke_result_t<F, T>;

    auto operator()(result::Result<T> result) -> result::Result<U> {
      if (result.has_value()) {
        return result.map(std::move(fun_));
      } else {
        return result::Err(result.error());
      }
    }
  };
} // namespace meijic::futures::mappers