#include "Server.h"
Server::Server(EventLoop* loop,int port)
    :loop_(loop),acceptor_(std::make_unique<Acceptor>(loop,port))
{
    std::cout << "Server create success!" << std::endl;
    //server调用callback设置tcp通信socket
    acceptor_->setNewConnectionCallback([this](int fd){newConnection(fd);});
}
Server::~Server() = default;
void Server::newConnection(int fd)
{
    auto conn =std::make_shared<TcpConnection>(loop_,fd);
    conn->setMessageCallback([this](auto conn){onMessage(conn);});
    conn->setCloseCallback([this](auto fd){removeConnection(fd);});
    connections_[fd] = conn;
}
void Server::removeConnection(int fd)
{
    std::cout << "remove connection fd = " << fd << std::endl;
    connections_.erase(fd);
}

void Server::onMessage(TcpConnection::TcpConnectionPtr conn)
{
    auto& buffer = conn->getInputBuffer();
    auto& context = conn->context();
    context.parseRequest(buffer);
}