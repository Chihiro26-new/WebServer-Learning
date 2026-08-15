#include "EventLoopThreadPool.h"
EventLoopThreadPool::
EventLoopThreadPool(EventLoop* baseLoop, int numThreads):
baseLoop_(baseLoop),
started_(false),
numThreads_(numThreads),
next_(0)
{

}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    if (loops_.empty())
        return baseLoop_;

    EventLoop* loop = loops_[next_];

    ++next_;

    if (next_ >= static_cast<int>(loops_.size()))
        next_ = 0;

    return loop;
}