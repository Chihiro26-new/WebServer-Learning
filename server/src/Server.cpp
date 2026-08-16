#include "Server.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "WorkerPool.h"
Server::Server(
    EventLoop* loop,
    WorkerPool* workerPool,
    int port
)
    : loop_(loop)
    , workerPool_(workerPool)
    , acceptor_(std::make_unique<Acceptor>(
          loop_,
          port
      ))
{
    acceptor_->setNewConnectionCallback(
        [this](int fd)
        {
            newConnection(fd);
        }
    );
}
Server::~Server() = default;
void Server::newConnection(int fd)
{
    workerPool_->dispatch(fd);
}
