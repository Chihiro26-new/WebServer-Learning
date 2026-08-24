#pragma once
#include"Condition.h"
#include "Thread.h"
#include "noncopyable.h"
class EventLoop;
class EventLoopThread:public noncopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();
    // 启动线程，并返回该线程中的EventLoop
    EventLoop* startLoop();
    void join();
private:
    void threadFunc();
    EventLoop* loop_;//
    Thread thread_;
    // 控制线程是否退出
    bool exiting_;
     // 保护 loop_ 等共享状态
    MutexLock mutex_;
    // 用于等待 / 通知 EventLoop 创建完成
    Condition cond_;
};