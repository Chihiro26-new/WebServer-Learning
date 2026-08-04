#pragma once
#include "Acceptor.h"
class Server
{
public:
    // 创建服务器  
    // loop: 当前服务器所属的事件循环 
    // port: 服务器监听端口
    Server(EventLoop* loop, int port);
    // 服务器析构，负责释放 Acceptor以及所有客户端连接
    ~Server();
    // 启动服务器事件循环
    void start();
private:
    //创建对应的 TcpConnection 并交给 Server管理
    void newConnection(int fd);
    // 客户端连接断开后
    // 从 Server 的连接管理表中移除对应的TcpConnection
    void removeConnection(int fd);
    void onMessage(TcpConnection::TcpConnectionPtr conn);
private:
    EventLoop* loop_;//EventLoop 驱动
    std::unique_ptr<Acceptor> acceptor_;// 管理所有客户端连接
    // 管理所有已经建立的客户端连接 
    // key: 客户端 socket fd 
    // value: 对应的TcpConnection
    std::unordered_map<int, std::shared_ptr<TcpConnection>> connections_;
};