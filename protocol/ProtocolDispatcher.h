#pragma once
#include "Buffer.h"
#include "ProtocolHandler.h"
#include "HttpHandler.h"
#include "RedisHandler.h"
#include <string.h>
class Buffer;
class ProtocolDispatcher
{
public:
    virtual ~ProtocolDispatcher()=0;
    virtual std::shared_ptr<ProtocolHandler>
    dispatch(const Buffer& buffer) = 0;

};

class MultiProtocolDispatcher:public ProtocolDispatcher
{
public:
    std::shared_ptr<ProtocolHandler>
dispatch(const Buffer& buffer) override
{
    if(isHttp(buffer))
    {
        return std::make_shared<HttpHandler>();
    }


    if(isRedis(buffer))
    {
        return std::make_shared<RedisHandler>();
    }

    return nullptr;
}
private:    
    bool isHttp(const Buffer&buffer)
    {
        auto data = buffer.peek();
        return buffer.readableBytes() >= 4
        &&
        (
          memcmp(data,"GET ",4)==0 ||
          memcmp(data,"POST",4)==0
        );
    };
    bool isRedis(const Buffer&buffer)
    {
        return buffer.readableBytes()>0
        &&
        buffer.peek()[0]=='*';
    }

};