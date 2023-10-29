#include <meijic/support/semaphore.hpp>
namespace sup
{
    template <class Tag>
    Semaphore<Tag>::Token::Token(Token &&that)
    {
        that.Invalidate();
    }

    template <class Tag>
    Semaphore<Tag>::Token::~Token()
    {
        assert(!valid_);
    }

    template <class Tag>
    void Semaphore<Tag>::Token::Invalidate()
    {
        assert(valid_);
        valid_ = false;
    }

    template <class Tag>
    typename Semaphore<Tag>::Token Semaphore<Tag>::Acquire()
    {
        std::unique_lock<std::mutex> guard(mutex_);
        while (available_tokens_ == 0)
        {
            cond_.wait(guard);
        }
        --available_tokens_;
        return Semaphore<Tag>::Token{};
    }

    template <class Tag>
    void Semaphore<Tag>::Release(Token &&token)
    {
        {
            std::unique_lock<std::mutex> guard(mutex_);
            ++available_tokens_;
        }
        cond_.notify_one();
        token.Invalidate();
    }
}