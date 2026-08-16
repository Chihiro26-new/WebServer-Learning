#pragma once
#include <memory>
#include "noncopyable.h"
class EventLoop;
class Acceptor;
class ProtocolHandler;
class TcpConnection;
class EventLoopThreadPool;
class WorkerPool;

class Server : public noncopyable
{
public:
    Server(
        EventLoop* loop,
        WorkerPool* workerPool,
        int port
    );

    ~Server();
    void start();

private:
    // accept 新连接，并将 fd 分发给 Worker
    void newConnection(int fd);

private:
    // 主 EventLoop
    //
    // 负责：
    // 1. 驱动 Acceptor
    // 2. 接收新连接
    // 3. 将 fd 分发给 WorkerPool
    EventLoop* loop_;

    // Worker 池
    //
    // Server 不直接管理 TcpConnection。
    // 连接建立后，由 Worker 接管其完整生命周期。
    WorkerPool* workerPool_;

    // 监听 socket
    std::unique_ptr<Acceptor> acceptor_;
};