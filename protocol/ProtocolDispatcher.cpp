#include"ProtocolDispatcher.h"
#include "HttpHandler.h"
#include "RedisHandler.h"
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
        return std::make_shared<HttpHandler>(
        staticHandler_);
    }
    if(isRedis(buffer))
    {
        return std::make_shared<RedisHandler>();
    }

    return nullptr;
}

 bool MultiProtocolDispatcher::isHttp(const Buffer&buffer)
{
    auto data = buffer.peek();
    return buffer.readableBytes() >= 4
    &&
    (
        memcmp(data,"GET ",4)==0 ||
        memcmp(data,"POST",4)==0
    );
    std::cout<<"http"<<std::endl;
};
bool MultiProtocolDispatcher::isRedis(const Buffer&buffer)
{
    return buffer.readableBytes()>0
    &&
    buffer.peek()[0]=='*';
}