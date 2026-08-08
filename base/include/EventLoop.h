#pragma once
#include "Epoll.h"
#include "Timer.h"
#include "TimerId.h"
#include "noncopyable.h"
class EventLoop :public noncopyable
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void removeChannel(Channel* channel);
    void addChannel(Channel* channel);
    int getTimeout(const TimePoint& expire);
    void cancelTimer(TimerId timerId_);
    TimerId addTimer(TimePoint expire,Timer::Callback cb);
private:
    Epoll epoll_;
    TimerQueue timerQueue_;//定时器
};