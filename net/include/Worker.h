#pragma once
#include "EventLoopThread.h"

class TcpConnection;
class ProtocolHandler;
class EventLoop;
class Worker : noncopyable
{
public:
    explicit Worker(
        std::shared_ptr<ProtocolHandler> handler
    );

    void start();
    void join();
    void shutdown();
    void addConnection(int fd);
    void removeConnection(int fd);
private:
    EventLoopThread loopThread_;

    EventLoop* loop_;

    std::shared_ptr<ProtocolHandler> handler_;

    std::unordered_map<
        int,
        std::shared_ptr<TcpConnection>
    > connections_;
};