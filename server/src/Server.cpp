#include "Server.h"


Server::Server(EventLoop* loop,int port)
    :
    loop_(loop),acceptor_(std::make_unique<Acceptor>(loop,port))
{
    std::cout << " Server create success!" << std::endl;
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
    auto conn =
        std::make_shared<TcpConnection>(
            loop_,
            fd
        );
    connections_[fd] = conn;
}
void Server::removeConnection(int fd)
{
    connections_.erase(fd);
}