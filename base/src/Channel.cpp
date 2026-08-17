#include "Channel.h"
#include <cstdint>
#include <sys/epoll.h>
#include <iostream>

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      closed_(false),
      events_(EPOLLIN),
      revents_(0),
      lastEvents_(0){
}

uint32_t Channel::getEvents()const{return events_;}
uint32_t Channel::getREvents()const{return revents_;}
uint32_t Channel::getLastEvents()const{return lastEvents_;}

int Channel::getFd() const{return fd_;}
void Channel::setFd(int fd){fd_=fd;}
void Channel::setClosed(){ closed_ = true;}
bool Channel::isClosed(){return closed_;};
void Channel::setReadHandler(CallBack cb)
{
    readHandler_ = std::move(cb);
}

void Channel::setWriteHandler(CallBack cb)
{
    writeHandler_ = std::move(cb);
}

void Channel::setErrorHandler(CallBack cb)
{
    errorHandler_ = std::move(cb);
}

void Channel::setConnHandler(CallBack cb)
{
    connHandler_ = std::move(cb);
}
void Channel::setCloseHandler(CallBack cb)
{
    closeHandler_=std::move(cb);
}
bool Channel::hasEventsChanged()
{
    return lastEvents_ != events_;
}

void Channel::updateLastEvents()
{
    lastEvents_=events_;
}
void Channel::enableWriting()
{
    events_ |= EPOLLOUT;
    // std::cout<<"enable EPOLLOUT events="
    //          <<events_
    //          <<std::endl;
}
void Channel::enableReading()
{
    events_|=EPOLLIN;
}

void Channel::disableWriting()
{
    events_ &= ~EPOLLOUT;
}

void Channel::disableReading()
{
    events_&=~EPOLLIN;
}
void Channel::disableAll()
{
    events_=0;
}
void Channel::setRevents(uint32_t events)
{
    revents_ = events;
}

void Channel::setEvents(uint32_t events)
{
    events_ = events;
}

void Channel::handleEvents()
{
        // socket错误
        if (revents_ & EPOLLERR)
        {
            // std::cout<< "Channel handleEvents:Error\n";
            if(errorHandler_)
                errorHandler_();
            return;
        }
        // 可读事件
        if (revents_ & (EPOLLIN | EPOLLPRI))
        {
            // std::cout<< "Channel handleEvents:Read\n";
            if(readHandler_)
                readHandler_();
            if(closed_)
                return;
        }

        // 对端关闭
        if (revents_ & (EPOLLRDHUP | EPOLLHUP))
        {
            // std::cout<< "Channel handleEvents:Close\n";
            if(closeHandler_)
                closeHandler_();
            return;
        }
        // 可写事件
        if (revents_ & EPOLLOUT)
        {
            // std::cout<< "Channel handleEvents:Write\n";
            if(writeHandler_)
                writeHandler_();
        }
}
