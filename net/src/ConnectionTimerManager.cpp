#include "ConnectionTimerManager.h"
#include "TcpConnection.h"
#include "EventLoop.h"
ConnectionTimerManager::ConnectionTimerManager(EventLoop* loop)
    :
    loop_(loop)
{
    timers_.fill(TimerId());
}

void ConnectionTimerManager::setConnection(
        std::weak_ptr<TcpConnection> conn)
{
    conn_ = std::move(conn);
}
TimerId ConnectionTimerManager::createTimer(
        TimeoutType type,
        TimePoint expire,
        Timer::Callback cb)
{
    size_t index =
        static_cast<size_t>(type);
    auto id = loop_->addTimer(
        expire,
        [this,index,cb]()
        {
            timers_[index] = TimerId();
            cb();
        }
    );

    timers_[index] = id;
    return id;
}

void ConnectionTimerManager::startIdleTimer()
{
    createTimer(
        TimeoutType::Idle,
        Clock::now()+std::chrono::seconds(5),
        [weakConn = conn_]()
        {
            if(auto conn = weakConn.lock())
            {
                conn->forceClose(
                    CloseReason::Timeout
                );
            }
        }
    );
}


void ConnectionTimerManager::refreshIdleTimer()
{
    cancel(TimeoutType::Idle);
    startIdleTimer();
}


void ConnectionTimerManager::startCloseTimer()
{
    createTimer(
        TimeoutType::Close,
        Clock::now()+std::chrono::seconds(10),
        [weakConn = conn_]()
        {
            if(auto conn = weakConn.lock())
            {
                conn->forceClose(
                    CloseReason::Timeout
                );
            }
        }
    );
}

void ConnectionTimerManager::startHeartbeatTimer()
{
    createTimer(
        TimeoutType::Heartbeat,
        Clock::now()+std::chrono::seconds(30),
        [weakConn = conn_]()
        {
            if(auto conn = weakConn.lock())
            {
                /*
                    后续:
                    conn->sendHeartbeat();
                    refreshHeartbeatTimer();
                */
                conn->forceClose(
                    CloseReason::Timeout
                );
            }
        }
    );
}



void ConnectionTimerManager::cancel(
        TimeoutType type)
{
    auto index =
        static_cast<size_t>(type);

    //  std::cout
    //     << "cancel timer type="
    //     << index
    //     << std::endl;
    auto& timer =
        timers_[index];

    if(timer.valid())
    {
        loop_->cancelTimer(timer);
        timer = TimerId();
    }
}





void ConnectionTimerManager::cancelAll()
{
    for(auto& timer : timers_)
    {
        if(timer.valid())
        {
            loop_->cancelTimer(timer);

            timer = TimerId();
        }
        else
        {
            // std::cout<<"timer invalid"<<std::endl;
        }
    }
}