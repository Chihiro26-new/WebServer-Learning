#pragma once
#include "Epoll.h"
#include "Timer.h"
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void removeChannel(Channel* channel);
    void addChannel(Channel* channel);
    int getTimeout(const TimePoint& expire);
    void addTimer(TimePoint expire,Timer::Callback cb);
private:
    Epoll epoll_;
    TimerQueue timerQueue_;//定时器
};