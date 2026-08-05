#pragma once
#include <memory>
#include "Buffer.h"
#include "Socket.h"
#include <functional>
class ProtocolHandler;
class Channel;
class EventLoop;
class TcpConnection:
    public std::enable_shared_from_this<TcpConnection>
{
public:
    using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
    using CloseCallback = std::function<void(int)>;
    
public:
    TcpConnection(EventLoop* loop,int fd);
    ~TcpConnection();

    Buffer& getInputBuffer();
    Buffer& getOutputBuffer();
    void setProtocolHandler(std::shared_ptr<ProtocolHandler> handle);
    void sendMsg(const std::string&msg);
    void setCloseCallback(CloseCallback cb);//通知上层server关闭tcp流
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();
    void handleConn();
    void connectEstablished();
    void startTimeout();
private:
    CloseCallback closeCallback_;
    Socket socket_;
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    Buffer inputBuffer_;//读缓冲
    Buffer outputBuffer_;//写缓冲
    std::shared_ptr<ProtocolHandler>handler_;
};
