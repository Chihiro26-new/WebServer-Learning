#include "Channel.h"
#include <cstdint>

uint32_t Channel::getEvents()const 
{
    return events_;
}

int Channel::getFd() const
{
    return fd_;
}