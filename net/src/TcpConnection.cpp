#include "TcpConnection.h"
#include "ProtocolHandler.h"
#include <sys/types.h>
#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"
#include <sys/socket.h>
#include <cassert>
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
    assert(state_ == ConnectionState::Disconnected);
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
void TcpConnection::enableOutput()
{
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
    if(outputBuffer_.readableBytes()==0)
    {
        handleClose();
    }
    else
    {
        channel_->enableWriting();
    }
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
    state_=state;
}

void TcpConnection::setCloseCallback(CloseCallback cb)
{
    closeCallback_ = std::move(cb);
}

void TcpConnection::maybeShrinkBuffer()
{
    if(inputBuffer_.size()> 1024 * 1024 &&
       inputBuffer_.readableBytes() < 4096)
    {
        inputBuffer_.shrink();
    }
}
void TcpConnection::handleRead()
{
    ssize_t n=inputBuffer_.readFd(socket_.getfd());

    if(n>0)
    {
        timerManager_.refreshIdleTimer();
        // std::cout<<"call message callback\n";
        if(handler_)
        {
            handler_->onMessage(shared_from_this());
            maybeShrinkBuffer();
        }
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        handleError();
    }
}

void TcpConnection::shutdown()
{
    if(state_==ConnectionState::Disconnecting)
        socket_.shutdownWrite();
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
        shutdown();
    }
}

void TcpConnection::handleClose()
{
    auto self = shared_from_this();

    if(state_ == ConnectionState::Disconnected)
        return;

    setState(ConnectionState::Disconnected);

    if(handler_)
    {
        handler_->onClose(self);
    }

    timerManager_.cancelAll();

    channel_->disableAll();
    channel_->setClosed();

    loop_->removeChannel(channel_.get());

    if(closeCallback_)
    {
        closeCallback_(socket_.getfd());
    }
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
    forceClose(CloseReason::Error);
}
void TcpConnection::handleConn()
{
    // std::cout<<"channal connection established"<<std::endl;
}

void TcpConnection::connectEstablished()
{
    auto self = shared_from_this();


    timerManager_.setConnection(self);

    setState(ConnectionState::Connected);

    timerManager_.startIdleTimer();

    if(handler_)
    {
        handler_->onConnection(self);
    }
}

