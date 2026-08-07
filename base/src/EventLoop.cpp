#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include "TimerId.h"

EventLoop::EventLoop()
{
}

EventLoop::~EventLoop(){}

int EventLoop::getTimeout(const TimePoint& expire)
{
    if(expire == TimePoint::max())
    {
        return -1;
    }
    auto now = Clock::now();
    if(expire <= now)
    {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        expire - now
    ).count();
}
void EventLoop::cancelTimer(TimerId timerId_)
{
    timerQueue_.cancel(timerId_);
}
TimerId EventLoop::addTimer(TimePoint expire,Timer::Callback cb)
{
    auto timer = std::make_shared<Timer>(
        expire,
        std::move(cb)
    );
    return timerQueue_.add(timer);
}
void EventLoop::loop()
{
    while(true)
    {
        auto expire = timerQueue_.getNextExpire();
        int timeout = getTimeout(expire);
            // std::cout<<"epoll wait timeout = "
            //  <<timeout
            //  <<std::endl;
        auto channels = epoll_.poll(timeout);
        for(auto c:channels)
        {
            c->handleEvents();
            if(c->isClosed())
                continue;
            if(c->hasEventsChanged())
            {
                epoll_.modify(c);
            }
        }
        timerQueue_.handleExpired();
    }
}
void EventLoop::removeChannel(Channel*channel){epoll_.remove(channel);};
void EventLoop::addChannel(Channel* channel){epoll_.add(channel);};