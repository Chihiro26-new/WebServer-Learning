#include "TcpConnection.h"
#include "ProtocolHandler.h"
#include <sys/types.h>
#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"
#include <sys/socket.h>
#include <iostream>
#include <string.h>
const int BUFSIZE=1024;
TcpConnection::TcpConnection(EventLoop*loop,int fd)
    :socket_(fd),loop_(loop),channel_(std::make_unique<Channel>(loop,fd))
    ,state_(ConnectionState::Connecting),closeReason_(CloseReason::None)
    ,timerManager_(loop)
{
    channel_->setReadHandler([this](){handleRead();});
    channel_->setWriteHandler([this](){handleWrite();});
    channel_->setCloseHandler([this](){handleClose();});
    channel_->setErrorHandler([this](){handleError();});
    channel_->setConnHandler([this](){handleConn();});
    loop_->addChannel(channel_.get());
}
TcpConnection::~TcpConnection()
{
    
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
    if(state_ != ConnectionState::Connected)
    {
        return;
    }
    outputBuffer_.append(msg);
    channel_->enableWriting();
}
void TcpConnection::startDisconnect()
{
    if(state_!=ConnectionState::Connected)
    {
        return;
    }
    closeReason_=CloseReason::ServerClose;
    setState(ConnectionState::Disconnecting);
    timerManager_.startCloseTimer();
}

void TcpConnection::forceClose(CloseReason reason)
{
    if(state_ == ConnectionState::Disconnected)
            return;
    closeReason_=reason;
    handleClose();
}
void TcpConnection::setState(ConnectionState state)
{
    // std::cout
    //     << "fd="
    //     << socket_.getfd()
    //     << " state "
    //     << static_cast<int>(state_)
    //     << " -> "
    //     << static_cast<int>(state)
    //     << std::endl;
    state_=state;
}

void TcpConnection::setCloseCallback(CloseCallback cb)
{
    closeCallback_ = std::move(cb);
}

void TcpConnection::handleRead()
{
    ssize_t n=inputBuffer_.readFd(socket_.getfd());
    // std::cout 
    // <<"read bytes="
    // <<n
    // <<" buffer readable="
    // <<inputBuffer_.readableBytes()
    // <<std::endl;
    if(n>0)
    {
        timerManager_.refreshIdleTimer();
        // std::cout<<"call message callback\n";
        if(handler_)
        {
            handler_->onMessage(shared_from_this());
        }
    }
    else if(n == 0)
    {
        // std::cout<<"client closed\n";
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
     while(outputBuffer_.readableBytes()>0)
    {
        ssize_t n = send(
            socket_.getfd(),
            outputBuffer_.peek(),
            outputBuffer_.readableBytes(),
            MSG_NOSIGNAL
        );

        if(n > 0)
        {
            outputBuffer_.retrieve(n);
            timerManager_.refreshIdleTimer();
        }
        else
        {
            if(errno == EAGAIN ||
               errno == EWOULDBLOCK)
            {
                return;
            }
            handleError();
            return;
        }
    }
    channel_->disableWriting();
    if(state_ == ConnectionState::Disconnecting)
    {
        handleClose();
    }
}

void TcpConnection::handleClose()
{
    if(state_ == ConnectionState::Disconnected)
    {
        return;
    }
    // std::cout<<"Tcp connection close\n";
    setState(ConnectionState::Disconnected);
    if(handler_)
    {
        handler_->onClose(shared_from_this());
    }
    timerManager_.cancelAll();
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
    forceClose(CloseReason::Error);
}
void TcpConnection::handleConn()
{
    std::cout<<"channal connection established"<<std::endl;
}

void TcpConnection::connectEstablished()
{
    timerManager_.setConnection(
        shared_from_this()
    );
    setState(ConnectionState::Connected);
    timerManager_.startIdleTimer();
    if(handler_)
    {
        handler_->onConnection(
            shared_from_this()
        );
    }
}

