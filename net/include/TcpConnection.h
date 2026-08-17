#pragma once
#include <memory>
#include "Buffer.h"
#include "ConnectionTimerManager.h"
#include "Socket.h"
#include <functional>
class ProtocolHandler;
class Channel;
class EventLoop;
class ConnectionTimerManager;
enum class ConnectionState
{
    Connecting,
    Connected,
    Disconnecting,//决定关闭,还有事情没完成
    Disconnected//真正关闭完成
};
enum class CloseReason
{
    None,
    ClientClose,
    ServerClose,
    Timeout,
    Error
};

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
    void enableOutput();
    void startDisconnect();
    void forceClose(CloseReason reason);
    void setCloseCallback(CloseCallback cb);//通知上层server关闭tcp流
    void connectEstablished();

private:

    void setState(ConnectionState);
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();
    void handleConn();
    void maybeShrinkBuffer();
private:
   
    CloseCallback closeCallback_;
    Socket socket_;
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_;
    Buffer inputBuffer_;//读缓冲
    Buffer outputBuffer_;//写缓冲
    std::shared_ptr<ProtocolHandler>handler_;
    ConnectionState state_;//当前 TCP 生命周期状态
    CloseReason closeReason_;//关闭原因
    ConnectionTimerManager timerManager_;   
};
