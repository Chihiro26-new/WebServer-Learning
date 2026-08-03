#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include <memory>
#include <string>
#include "Socket.h"
#include <sys/socket.h>
class EventLoop;
class Channel;
class TcpConnection
{
public:
    TcpConnection(EventLoop* loop,int fd);
    ~TcpConnection();
    void handleRead();
    void handleWrite();
private:
    Socket socket_;
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    std::string readBuffer_;
    std::string writeBuffer_;
};
