#pragma once
#include <functional>
#include <queue>
#include <memory>
#include <chrono>
#include "TimerId.h"
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
class Timer
{
public:
    using Callback = std::function<void()>;
    Timer(TimePoint expireTime, Callback cb);
    ~Timer();
    bool isExpired(TimePoint now) const;
    void run();
    TimePoint  expireTime() const;
    void cancel();
    bool canceled() const;
private:
    TimePoint expireTime_;
    Callback callback_;
    bool canceled_{false};
};

struct TimerCmp
{
    bool operator()(const std::shared_ptr<Timer>& a,
                    const std::shared_ptr<Timer>& b)
    {
        if(!a)
            return false;
        if(!b)
            return true;
        return a->expireTime() >b->expireTime();
        
    }
};

class TimerQueue
{
public:
    TimerQueue();
    ~TimerQueue();
    void cancel(TimerId timerId);//取消任务
    TimerId add(std::shared_ptr<Timer> timer);//添加Timer任务
    void handleExpired();//处理
    TimePoint getNextExpire();//获取最近Timer
private:
    std::priority_queue<std::shared_ptr<Timer>
    ,std::vector<std::shared_ptr<Timer>>
    ,TimerCmp>timers_;//小根堆
};
