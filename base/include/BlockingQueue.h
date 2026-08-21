#pragma once
#include "noncopyable.h"
#include "Mutex.h"
#include "Condition.h"
#include <deque>

template<typename T>
class BlockingQueue : noncopyable
{
public:

    BlockingQueue()
        :
        mutex_(),
        cond_(mutex_)
    {

    }


    void put(const T& value)
    {
        {
            LockGuard lock(mutex_);

            queue_.push_back(value);
        }

        cond_.notify();
    }


    T take()
    {
        LockGuard lock(mutex_);

        while(queue_.empty())
        {
            cond_.wait();
        }

        T value = queue_.front();

        queue_.pop_front();

        return value;
    }


private:
    MutexLock mutex_;
    Condition cond_;
    std::deque<T> queue_;
};