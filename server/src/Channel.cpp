#include "Channel.h"
#include <cstdint>
#include <sys/epoll.h>
#include <iostream>
uint32_t Channel::getEvents()const {return events_;}

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(EPOLLIN),
      revents_(0),
      lastEvents_(0){}

int Channel::getFd() const{return fd_;}

void Channel::setFd(int fd){fd_=fd;}

uint32_t Channel::getREvents()const{return revents_;}


void Channel::setReadHandler(CallBack cb)
{
    readHandler_ = std::move(cb);
}
bool Channel::hasEventsChanged()
{
    return lastEvents_ != events_;
}

void Channel::updateLastEvents()
{
    lastEvents_=events_;
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

void Channel::enableWriting()
{
    events_ |= EPOLLOUT;
}

void Channel::disableWriting()
{
    events_ &= ~EPOLLOUT;
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
    std::cout << "Channel handleEvents, revents = "<< revents_
          << std::endl;
    if(revents_&EPOLLERR){
        if(errorHandler_)
            errorHandler_();
    }
    if(revents_&EPOLLIN){
        if(readHandler_)
            readHandler_();
    }
    if(revents_ & EPOLLOUT){
        if(writeHandler_)
            writeHandler_();
    }
}
