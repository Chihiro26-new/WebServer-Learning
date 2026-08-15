#pragma once
#include "Channel.h"
#include "Epoll.h"
#include "Timer.h"
#include "TimerId.h"
#include "noncopyable.h"
#include <thread>
#include <mutex>
class EventLoop :public noncopyable
{
public:
    EventLoop();
    ~EventLoop();

    using Functor=std::function<void()>;
    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);
    bool isInLoopThread()const;

    void loop();
    void quit();
    void removeChannel(Channel* channel);
    void addChannel(Channel* channel);
    int getTimeout(const TimePoint& expire);
    void cancelTimer(TimerId timerId_);
    TimerId addTimer(TimePoint expire,Timer::Callback cb);
private:
    Epoll epoll_;
    TimerQueue timerQueue_;//定时器
    int wakeupFd_;
    std::unique_ptr<Channel>wakeupChannel_;

    std::mutex mutex_;
    std::vector<Functor> pendingFunctors_;

    bool quit_;
    std::thread::id threadId_;
    
    void doPendingFunctors();
    void handleRead(); // 处理 wakeupFd_ 的可读事件，消费唤醒通知
    void wakeup();// 向 wakeupFd_ 写入事件，唤醒阻塞中的 EventLoop
};