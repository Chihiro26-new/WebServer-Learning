#include "EventLoopThread.h"
#include "EventLoop.h"
#include <mutex>
#include <iostream>
EventLoopThread::EventLoopThread():
loop_(nullptr),
thread_([this]() {
        threadFunc();
    },
    "EventLoopThread"),
exiting_(false),
mutex_(),
cond_()
{

}

EventLoopThread::~EventLoopThread()
{
    exiting_=true;
    EventLoop* loop = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop = loop_;
    }

    if (loop != nullptr)
    {
        loop->quit();
    }
    thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
    thread_.start();
    std::unique_lock<std::mutex>lock(mutex_);
    while(loop_==nullptr)
    {
        cond_.wait(lock);
    }
    return loop_;
}
void EventLoopThread::join()
{
    thread_.join();
}
void EventLoopThread::threadFunc()
{
    // std::cout << "EventLoopThread: thread started\n";
    EventLoop loop;
    {
        std::lock_guard<std::mutex>lock(mutex_);
        loop_=&loop;
    }
    cond_.notify_one();
    // std::cout << "EventLoopThread: loop start\n";
    loop.loop();
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop_ = nullptr;
    }
}