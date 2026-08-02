#pragma once
#include <sys/epoll.h>
#include "Channel.h"
#include <vector>
#include <unordered_map>
class Epoll
{
    public:
        Epoll();
        ~Epoll();
        void add(Channel* channel);
        void modify(Channel* channel);
        void remove(Channel* channel);
        std::vector<Channel*> poll(int timeout);

    private:
    int epollFd_;
    std::vector<epoll_event> events_;
    std::unordered_map<int,Channel*> channels_;
};