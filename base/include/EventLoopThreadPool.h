#pragma once
#include "EventLoopThread.h"
#include "noncopyable.h"
#include <vector>
class EventLoop;
class EventLoopThreadPool:noncopyable
{
public:
    EventLoopThreadPool(EventLoop* baseLoop, int numThreads);
    void start();
    EventLoop* getNextLoop();
private:
     EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    size_t next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;

};