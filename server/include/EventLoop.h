#pragma once
#include "Channel.h"
#include "Epoll.h"
class Epoll;
class EventLoop
{
    public:
    EventLoop();
    ~EventLoop();
    void loop();

    void removeChannel(Channel* channel);
    void addChannel(Channel* channel);

    private:
    Epoll epoll_;

};