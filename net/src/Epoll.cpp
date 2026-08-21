#include "Epoll.h"
#include "Channel.h"
#include <assert.h>
#include <sys/epoll.h>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <iostream>
const int EventsNum=4096;
Epoll::Epoll():epollFd_(epoll_create1(EPOLL_CLOEXEC)), 
    events_(EventsNum)
{
    assert(epollFd_ > 0);
}
Epoll::~Epoll() {
    close(epollFd_);
}
int Epoll::getEpollFd()const{return epollFd_;};
void Epoll::add(Channel*channel)
{
    int fd=channel->getFd();
    //   std::cout
    //     <<"epoll add fd="
    //     <<fd
    //     <<std::endl;

    epoll_event event{};
    event.data.fd=fd;
    event.events=channel->getEvents();

    if(epoll_ctl(
        epollFd_,
        EPOLL_CTL_ADD,
        fd,
        &event)<0)
    {
        perror("epoll_add error");
        return;
    }
    channels_[fd] = channel;
}

void Epoll::modify(Channel* channel)
{
    int fd = channel->getFd();
    epoll_event event{};
    event.data.fd = fd;
    event.events = channel->getEvents();
    if(epoll_ctl(
        epollFd_,
        EPOLL_CTL_MOD,
        fd,
        &event)<0)
        {
            perror("epoll_modify error");
            return;
        }
    channel->updateLastEvents();//更新修改
}
void Epoll::remove(Channel* channel)
{
    int fd=channel->getFd();
    if(epoll_ctl(epollFd_,EPOLL_CTL_DEL,fd,nullptr)<0)
    {
        perror("epoll_remove error.");
        return;
    }
    channels_.erase(fd);
}

std::vector<Channel*> Epoll::poll(int timeout)
{
    std::vector<Channel*>activeChannels;
    int eventNum=epoll_wait(epollFd_,
        events_.data(),
        events_.size(),
        timeout);
    for(int i=0;i<eventNum;++i)
    {
        int fd=events_[i].data.fd;
        Channel* channel = channels_[fd];
        channel->setRevents(events_[i].events);
        activeChannels.push_back(channel);
    }
    return activeChannels;
}