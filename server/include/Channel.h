#pragma once
#include <cstdint>
class EventLoop;
class Channel
{
    public:
        Channel(EventLoop* loop, int fd);

        uint32_t getEvents()const;
        int getFd() const;
    private:

        EventLoop* loop_;
        int fd_;
        uint32_t events_;
        uint32_t revents_;
};