#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
const int BUFSIZE=1024;
TcpConnection::TcpConnection(EventLoop*loop,int fd)
    :socket_(fd),loop_(loop),channel_(std::make_shared<Channel>(loop,fd))
{
    std::cout<<"Tcp creat success!"<<std::endl;
    channel_->setReadHandler([this](){handleRead();});
    channel_->setWriteHandler([this](){handleWrite();});
    channel_->setCloseHandler([this](){handleClose();});
    channel_->setErrorHandler([this](){handleError();});
    channel_->setConnHandler([this](){handleConn();});
    loop_->addChannel(channel_.get());
}
TcpConnection::~TcpConnection()
{
    std::cout << "TcpConnection destructor" << std::endl;
    loop_->removeChannel(channel_.get());
}
 void TcpConnection::setCloseCallback(CloseCallback cb)
{
    closeCallback_ = std::move(cb);
}
void TcpConnection::handleRead()
{
    std::cout << "handleRead called" << std::endl;
    char buf[BUFSIZE];
    int n = recv(socket_.getfd(),buf,sizeof(buf),0);
    if(n>0)
    {
        std::string msg(buf,n);
        std::cout<<msg<<std::endl;
        writeBuffer_ += msg;
        channel_->enableWriting();
    }
    else if(n == 0)
    {
        std::cout<<"client closed"<<std::endl;
        handleClose();
    }
    else
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    if(writeBuffer_.empty())
    {
        channel_->disableWriting();
        return;
    }
    int n = send(socket_.getfd(),
                 writeBuffer_.data(),
                 writeBuffer_.size(),
                 0);
    if(n > 0)
    {
        std::cout<<writeBuffer_.substr(0,n)<<std::endl;
        writeBuffer_.erase(0, n);
    }
    if(writeBuffer_.empty()){
        channel_->disableWriting();
    }
}

void TcpConnection::handleClose()
{
    std::cout<<"connection close"<<std::endl;
    if(closeCallback_)
        closeCallback_(socket_.getfd());
}
void TcpConnection::handleError()
{
    int err = 0;
    socklen_t len = sizeof(err);
    getsockopt(
        socket_.getfd(),
        SOL_SOCKET,
        SO_ERROR,
        &err,
        &len
    );
    std::cerr << "socket error: "<< strerror(err)<< std::endl;
    handleClose();
}
void TcpConnection::handleConn()
{
    std::cout<<"connection established"<<std::endl;
}