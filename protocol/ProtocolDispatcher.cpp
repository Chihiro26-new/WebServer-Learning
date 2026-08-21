#include"ProtocolDispatcher.h"
#include "HttpHandler.h"
#include "RedisHandler.h"
#include <string.h>
#include "Buffer.h"
std::shared_ptr<ProtocolHandler>MultiProtocolDispatcher::
dispatch(const Buffer& buffer) 
{
    if(isHttp(buffer))
    {
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
};
bool MultiProtocolDispatcher::isRedis(const Buffer&buffer)
{
    return buffer.readableBytes()>0
    &&
    buffer.peek()[0]=='*';
}