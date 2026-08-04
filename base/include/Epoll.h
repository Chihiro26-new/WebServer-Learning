#pragma once
#include <sys/epoll.h>
#include "Channel.h"
#include <vector>
#include <iostream>
#include <unordered_map>
class Epoll
{
public:
    Epoll();
    ~Epoll();
    int getEpollFd()const;
    void add(Channel* channel);
    void modify(Channel* channel);
    void remove(Channel* channel);
    std::vector<Channel*> poll(int timeout);
private:
int epollFd_;
std::vector<epoll_event> events_;//epoll返回事件
std::unordered_map<int,Channel*> channels_;//映射channel的fd
};