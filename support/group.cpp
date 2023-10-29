#include "group.hpp"
namespace sup
{
    void Group::Add(size_t delta)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        counter_ += delta;
    }

    void Group::Done()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        --counter_;
        if (counter_ == 0)
        {
            if (sleeps_ > 1)
            {
                cond_.notify_all();
            }
            if (sleeps_ == 1)
            {
                cond_.notify_one();
            }
        }
    }

    void Group::Wait()
    {
        {
            std::unique_lock<std::mutex> guard(mutex_);
            ++sleeps_;

            cond_.wait(guard, [&]()
                       { return counter_ == 0; });

            --sleeps_;
        }
    }
} // namespace sup
