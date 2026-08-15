#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include "TimerId.h"
#include <sys/eventfd.h>
#include <stdexcept>
#include "Util.h"
#include <iostream>
#include <thread>
#include <utility>
static int createEventfd()
{
    int fd = ::eventfd(
        0,
        EFD_NONBLOCK | EFD_CLOEXEC
    );

    if (fd < 0)
    {
        throw std::runtime_error("eventfd failed");
    }
    return fd;
}
EventLoop::EventLoop()
    : epoll_()
    , timerQueue_()
    , wakeupFd_(createEventfd())
    , wakeupChannel_(nullptr)
    , quit_(false)
    ,threadId_(std::this_thread::get_id())
{
     wakeupChannel_ =
        std::make_unique<Channel>(
            this,
            wakeupFd_
        );

    wakeupChannel_->setReadHandler(
        [this]()
        {
            handleRead();
        }
    );
    wakeupChannel_->enableReading();
    addChannel(wakeupChannel_.get());
}

EventLoop::~EventLoop(){}
void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
}
void EventLoop::queueInLoop(Functor cb)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }
    wakeup();
}

bool EventLoop::isInLoopThread() const
{
    return std::this_thread::get_id() == threadId_;
}

int EventLoop::getTimeout(const TimePoint& expire)
{
    if(expire == TimePoint::max())
    {
        return -1;
    }
    auto now = Clock::now();
    if(expire <= now)
    {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        expire - now
    ).count();
}
void EventLoop::cancelTimer(TimerId timerId_)
{
    timerQueue_.cancel(timerId_);
}
TimerId EventLoop::addTimer(TimePoint expire,Timer::Callback cb)
{
    auto timer = std::make_shared<Timer>(
        expire,
        std::move(cb)
    );
    return timerQueue_.add(timer);
}
void EventLoop::loop()
{
    while(true)
    {
        auto expire = timerQueue_.getNextExpire();
        int timeout = getTimeout(expire);
            // std::cout<<"epoll wait timeout = "
            //  <<timeout
            //  <<std::endl;
        auto channels = epoll_.poll(timeout);
        for(auto c:channels)
        {
            c->handleEvents();
            if(c->isClosed())
                continue;
            if(c->hasEventsChanged())
            {
                epoll_.modify(c);
            }
        }
        timerQueue_.handleExpired();
    }
}

void EventLoop::quit()
{
    std::cout << "quit called\n";
    quit_ = true;
    if(!isInLoopThread())
        wakeup();
}
void EventLoop::handleRead()
{
    uint64_t one = 0;

    ssize_t n = readNBytes(
        wakeupFd_,
        &one,
        sizeof(one)
    );
     std::cout << "wakeup read: "
              << n
              << ", value = "
              << one
              << std::endl;
    if (n != sizeof(one))
    {
        perror("EventLoop::handleRead");
    }
}
void EventLoop::wakeup()
{
    uint64_t one = 1;
    
    ssize_t n = writeNBytes(
        wakeupFd_,
        &one,
        sizeof(one)
    );
    std::cout << "wakeup write = " << n << std::endl;
    if(n != sizeof(one))
    {
        perror("EventLoop::wakeup");
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (auto& cb : functors)
    {
        cb();
    }
}

void EventLoop::removeChannel(Channel*channel){epoll_.remove(channel);};
void EventLoop::addChannel(Channel* channel){epoll_.add(channel);};