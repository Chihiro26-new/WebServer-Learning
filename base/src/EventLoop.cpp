#include "EventLoop.h"
#include "Channel.h"
EventLoop::EventLoop(){std::cout<<"EventLoop creat success! "<<std::endl;}
EventLoop::~EventLoop(){}

void EventLoop::loop()
{
    while(true)
    {
        auto channels=epoll_.poll(-1);
        std::cout << "poll: "
                  << channels.size()
                  << std::endl;
        for(auto c:channels)
        {
            c->handleEvents();
            if(c->isClosed())
                continue;
            if(c->hasEventsChanged())
            {
                epoll_.modify(c);
            }
        }
    }
}
void EventLoop::removeChannel(Channel*channel){epoll_.remove(channel);};
void EventLoop::addChannel(Channel* channel){epoll_.add(channel);};