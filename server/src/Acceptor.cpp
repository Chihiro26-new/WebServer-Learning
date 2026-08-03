#include "Acceptor.h"
#include "Channel.h"
Acceptor::Acceptor(EventLoop* loop,int port):
    acceptSocket_(Socket::createListenSocket(port)),
    channel_(loop, acceptSocket_.getfd()),
    loop_(loop)
{
    std::cout << "Acceptor create success!" << std::endl;
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
    std::cout << "Acceptor handleRead!" << std::endl;
    int connfd = accept(
        acceptSocket_.getfd(),
        nullptr,
        nullptr
    );
    std::cout << "find the connfd = " << connfd << std::endl;
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