#include "TcpConnection.h"
#include "ProtocolHandler.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"
#include <sys/socket.h>
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
}

Buffer& TcpConnection::getInputBuffer()
{
    return inputBuffer_;
}
Buffer& TcpConnection::getOutputBuffer()
{
    return outputBuffer_;
}

void TcpConnection::setProtocolHandler(std::shared_ptr<ProtocolHandler> handler)
{
     handler_ = handler;
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

void TcpConnection::handleRead()
{
    auto self = shared_from_this();
    ssize_t n=inputBuffer_.readFd(socket_.getfd());
    if(n>0)
    {
        std::cout<<"call message callback\n";
        if(handler_)
        {
            handler_->onMessage(shared_from_this());
        }
    }
    else if(n == 0)
    {
        std::cout<<"client closed\n";
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
    std::cout<<"Tcp connection close\n";
    if(handler_)
    {
        handler_->onClose(shared_from_this());
    }
    channel_->setClosed();
    loop_->removeChannel(channel_.get());
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
    std::cout<<"channal connection established"<<std::endl;
}

void TcpConnection::connectEstablished()
{
    std::cout<<"Tcp connection established\n";
    if(handler_)
    {
        handler_->onConnection(shared_from_this());
    }
    //startTimeout();
}
void TcpConnection::startTimeout()
{
    auto weakSelf = weak_from_this();
    loop_->addTimer(
        Clock::now()+std::chrono::seconds(20),
        [weakSelf]()
        {
            std::cout<<"Tcp time out timer creat!"<<std::endl;
            if(auto self = weakSelf.lock())
            {
                std::cout<<"before close\n";
                self->handleClose();
                std::cout<<"after close\n";
            }
        }
    );
}