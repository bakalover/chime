#pragma once
#include <cstdlib>
#include <mutex>
#include <condition_variable>
namespace sup
{
    class Group
    {
    public:
        void Add(size_t delta);
        void Done();
        void Wait();

    private:
        std::mutex mutex_; // Transfer to lock-free + intrusive
        std::condition_variable cond_;
        size_t counter_{0}, sleeps_{0};
    };
} // namespace sup
