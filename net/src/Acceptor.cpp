#include "Acceptor.h"
#include "EventLoop.h"
#include <sys/socket.h>
#include <stdio.h>
Acceptor::Acceptor(EventLoop* loop,int port):
    acceptSocket_(Socket::createListenSocket(port)),
    channel_(loop, acceptSocket_.getfd()),
    loop_(loop)
{
    channel_.setReadHandler([this](){handleRead();});
    loop_->addChannel(&channel_);
}
Acceptor::~Acceptor() = default;
void Acceptor::setNewConnectionCallback(
    NewConnectionCallback cb)
{
    newConnectionCallback_ = std::move(cb);
}
void Acceptor::handleRead()
{
    int connfd = accept(
        acceptSocket_.getfd(),
        nullptr,
        nullptr
    );
    // std::cout << "find the connfd = " << connfd << std::endl;
    if(connfd < 0)
    {
        perror("accept");
        return;
    }
    if(newConnectionCallback_)
    {
        newConnectionCallback_(connfd);
    }
}