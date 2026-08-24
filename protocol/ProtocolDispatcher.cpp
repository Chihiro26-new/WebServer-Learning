#include"ProtocolDispatcher.h"
#include <string.h>
#include "Buffer.h"
#include <iostream>
ProtocolDispatcher::~ProtocolDispatcher()
{

}
std::shared_ptr<ProtocolHandler>MultiProtocolDispatcher::
dispatch(const Buffer& buffer) 
{
    // std::cout<<"dispatch called"<<std::endl;
    if(isHttp(buffer))
    {
        // std::cout<<"HTTP detected"<<std::endl;
        return httpHandler_;
    }
    if(isRedis(buffer))
    {
        return redisHandler_;
    }

    return nullptr;
}

bool MultiProtocolDispatcher::isHttp(const Buffer&buffer)
{
    auto data = buffer.peek();
    bool result =
        buffer.readableBytes() >= 4 &&
        (
            memcmp(data, "GET ", 4) == 0 ||
            memcmp(data, "POST", 4) == 0
        );

    // if (result)
    //     std::cout << "http" << std::endl;
    return result;
};
bool MultiProtocolDispatcher::isRedis(const Buffer&buffer)
{
    auto result=buffer.readableBytes()>0&&buffer.peek()[0]=='*';
    // if(result)
    //     std::cout<<"redis"<<"\n";
    return result;
}