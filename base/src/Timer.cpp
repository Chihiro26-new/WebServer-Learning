#include "Timer.h"
#include "TimerId.h"
#include <iostream>
Timer::Timer(TimePoint expireTime,Callback cb):
    expireTime_(expireTime),
    callback_(std::move(cb))
{
    // std::cout 
    //     << this
    //     << std::endl;
}
Timer::~Timer()
{
    //   std::cout 
    //     << this
    //     << std::endl;
}
bool Timer::isExpired(TimePoint now)const
{
    return now>=expireTime_;
}
void Timer::run()
{
    if(canceled_)
        return;
    if(callback_)
    {
        callback_();
    }
}
TimePoint Timer::expireTime()const
{
    return expireTime_;
}
void Timer::cancel()
{
    canceled_=true;
}
bool Timer::canceled() const
{
    return canceled_;
}


TimerQueue::TimerQueue(){};
TimerQueue::~TimerQueue(){};
TimerId TimerQueue::add(std::shared_ptr<Timer> timer)
{
    if(!timer)
        return TimerId();
    timers_.push(timer);
    return TimerId(timer);
}
void TimerQueue::cancel(const TimerId &timerId)
{
    if(auto timer = timerId.timer_.lock())
    {
        timer->cancel();
    }
}
void TimerQueue::handleExpired()
{   
    auto now = Clock::now();
    while(!timers_.empty())
    {
        auto timer = timers_.top();//查看堆顶元素
        // 删除空指针
        if(!timer)
        {
            timers_.pop();
            continue;
        }

        if(!timer->isExpired(now))
        {
            break;
        }
        timers_.pop();//到期后移除
        if(!timer->canceled())
        {
            continue;
        }
        timer->run();
    }
}
TimePoint TimerQueue::getNextExpire()
{
    while(!timers_.empty())
    {
        auto timer = timers_.top();
        if(!timer || timer->canceled())
        {
            timers_.pop();
            continue;
        }
        return timer->expireTime();
    }
    return TimePoint::max();
}

