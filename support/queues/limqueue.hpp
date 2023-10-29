#pragma once
#include <cstdlib>
#include <deque>
#include <meijic/support/semaphore.hpp>
#include <meijic/support/locks/spinlock.hpp>
namespace sup
{
    template <typename T>
    class MPMCLimitedQueue
    {
    public:
        explicit MPMCLimitedQueue(size_t cap) : produce_(cap), consume_(0)
        {
        }

        void Put(T obj);

        T Take();

    private:
        // Tags
        struct SomeTag;

    private:
        using Token = typename sup::Semaphore<T>::Token;
        Semaphore<T> produce_;
        Semaphore<T> consume_;
        SpinLock spinlock_;
        std::deque<T> buff_;
    };
} // namespace sup
