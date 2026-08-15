#pragma once
#include "Thread.h"
#include "noncopyable.h"
#include <condition_variable>
#include <mutex>
class EventLoop;
class EventLoopThread:public noncopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();
    // 启动线程，并返回该线程中的 EventLoop
    EventLoop* startLoop();
private:
    void threadFunc();
    EventLoop* loop_;
    Thread thread_;
    // 控制线程是否退出
    bool exiting_;
     // 保护 loop_ 等共享状态
    std::mutex mutex_;
    // 用于等待 / 通知 EventLoop 创建完成
    std::condition_variable cond_;
};