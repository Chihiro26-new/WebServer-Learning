#include "TcpConnection.h"
#include "EventLoop.h"
#include <climits>
#include <iostream>
#include <sys/types.h>
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
void TcpConnection::sendMsg(const std::string&msg)
{
    outputBuffer_.append(msg);
    channel_->enableWriting();
}
void TcpConnection::setCloseCallback(CloseCallback cb)
{
    closeCallback_ = std::move(cb);
}
void TcpConnection::setMessageCallback(MessageCallback cb)
{
    messageCallback_=std::move(cb);   
}
void TcpConnection::handleRead()
{
    std::cout << "handleRead called" << std::endl;
    ssize_t n=inputBuffer_.readFd(socket_.getfd());
    if(n>0)
    {
        if(messageCallback_)
            messageCallback_(shared_from_this());
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
    if(outputBuffer_.readableBytes()==0)
    {
        channel_->disableWriting();
        return;
    }
    ssize_t n=send(socket_.getfd(),
                  outputBuffer_.peek(),
                    outputBuffer_.readableBytes(),
                0);

    if(n>0)
        outputBuffer_.retrieve(n);
    if(outputBuffer_.readableBytes()==0)
        channel_->disableWriting();
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