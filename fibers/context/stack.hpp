#pragma once
#include <cstddef>
#include <cstdlib>
namespace fibers::coro {
//TODO: More Abstracitons / mb ResourseManager
class Stack {
  public:
    char *End() { return start_ + size_; }

    Stack(std::size_t at_least_) { start_ = (char *)malloc(at_least_); }

    ~Stack() { free(start_); }
    
   private:
     char *start_;
     size_t size_;
};
} // namespace fibers::coro::context
