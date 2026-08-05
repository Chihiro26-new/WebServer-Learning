#include "Timer.h"
#include <iostream>
Timer::Timer(TimePoint expireTime,Callback cb):
    expireTime_(expireTime),
    callback_(std::move(cb))
{
    std::cout<<"Timer create\n";
}
bool Timer::isExpired(TimePoint now)const
{
    return now>=expireTime_;
}
void Timer::run()
{
    if(callback_)
    {
        callback_();
    }
}
TimePoint Timer::expireTime()const
{
    return expireTime_;
}

TimerQueue::TimerQueue(){};
TimerQueue::~TimerQueue(){};
void TimerQueue::addTimer(std::shared_ptr<Timer>timer)
{
    timers_.push(timer);
}
void TimerQueue::handleExpired()
{   
    auto now = Clock::now();
    while(!timers_.empty())
    {
        auto timer = timers_.top();

        if(!timer->isExpired(now))
        {
            break;
        }
        timers_.pop();
        timer->run();
    }

}
TimePoint TimerQueue::getNextExpire()
{
    if(timers_.empty())
    {
        return TimePoint::max();
    }
    std::cout<<"next expire\n";
    return timers_.top()->expireTime();
}