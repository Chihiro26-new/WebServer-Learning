#pragma once
#include <functional>
#include <queue>
#include <memory>
#include <chrono>
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
class Timer
{
public:
    using Callback = std::function<void()>;
    Timer(TimePoint expireTime, Callback cb);
    bool isExpired(TimePoint now) const;
    void run();
    TimePoint  expireTime() const;
private:
    TimePoint expireTime_;
    Callback callback_;
};

struct TimerCmp
{
    bool operator()(const std::shared_ptr<Timer>& a,
                    const std::shared_ptr<Timer>& b)
    {
        return a->expireTime() > b->expireTime();
    }
};

class TimerQueue
{
public:
    TimerQueue();
    ~TimerQueue();
    void addTimer(std::shared_ptr<Timer> timer);//添加任务
    void handleExpired();//处理
    TimePoint getNextExpire();//获取最近Timer
private:
    std::priority_queue<std::shared_ptr<Timer>
    ,std::vector<std::shared_ptr<Timer>>
    ,TimerCmp>timers_;//小根堆
};
