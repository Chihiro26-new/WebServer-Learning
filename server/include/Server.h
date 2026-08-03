#pragma once
#include "Acceptor.h"
class Server
{
public:
    Server(EventLoop* loop, int port);
    ~Server();
    void start();
private:
    void newConnection(int fd);
    void removeConnection(int fd);

private:
    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    // 管理所有客户端连接
    std::unordered_map<int, std::shared_ptr<TcpConnection>> connections_;
};