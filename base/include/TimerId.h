#pragma once
#include <memory>


class Timer;
class TimerQueue;
class TimerId
{
public:
    TimerId() = default;
    bool valid() const;//判断有效性
    
private:
    explicit TimerId(std::shared_ptr<Timer> timer)
        : timer_(timer){}
    std::weak_ptr<Timer> timer_;
    friend class TimerQueue;
};
