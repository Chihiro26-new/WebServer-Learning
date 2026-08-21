#pragma once
#include "EventLoopThread.h"
class TcpConnection;
class ProtocolDispatcher;
class Worker : noncopyable
{
public:
    explicit Worker(
        std::shared_ptr<ProtocolDispatcher> dispatcher
    );

    void start();
    void join();
    void shutdown();
    void addConnection(int fd);
    void removeConnection(int fd);
private:
    EventLoopThread loopThread_;

    EventLoop* loop_;
      //负责协议选择
    std::shared_ptr<ProtocolDispatcher>
        dispatcher_;
    std::unordered_map<
        int,
        std::shared_ptr<TcpConnection>
    > connections_;

};