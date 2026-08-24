#include "EventLoopThread.h"
#include "EventLoop.h"
EventLoopThread::EventLoopThread():
loop_(nullptr),
thread_([this]() {
        threadFunc();
    },
    "EventLoopThread"),
exiting_(false),
cond_(mutex_)
{

}

EventLoopThread::~EventLoopThread()
{
    exiting_=true;
    EventLoop* loop = nullptr;
    {
        LockGuard lock(mutex_);
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
    LockGuard lock(mutex_);
    while(loop_==nullptr)
    {
        cond_.wait();
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
        LockGuard lock(mutex_);
        loop_=&loop;
    }
    cond_.notify();
    // std::cout << "EventLoopThread: loop start\n";
    loop.loop();
    {
        LockGuard lock(mutex_);
        loop_ = nullptr;
    }
}