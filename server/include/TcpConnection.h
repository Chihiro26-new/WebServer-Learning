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
    using CloseCallback = std::function<void(int)>;
public:
    TcpConnection(EventLoop* loop,int fd);
    ~TcpConnection();
    void setCloseCallback(CloseCallback cb);//通知上层server关闭tcp流
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();
    void handleConn();
private:
    CloseCallback closeCallback_;
    Socket socket_;
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    std::string readBuffer_;//读缓冲
    std::string writeBuffer_;//写缓冲
};
