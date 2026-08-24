#pragma once
#include <memory>
class Buffer;
class ProtocolHandler;
class Buffer;
class StaticFileHandler;
class HttpHandler;
class RedisHandler;
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
    explicit MultiProtocolDispatcher(
        std::shared_ptr<ProtocolHandler> httpHandler,
        std::shared_ptr<ProtocolHandler> redisHandler
    )
    :
    httpHandler_(std::move(httpHandler)),
    redisHandler_(std::move(redisHandler))
    {

    }

    std::shared_ptr<ProtocolHandler>
    dispatch(const Buffer& buffer) override;
   
private:    
    bool isHttp(const Buffer&buffer);
    bool isRedis(const Buffer&buffer);
    std::shared_ptr<ProtocolHandler> httpHandler_;
    std::shared_ptr<ProtocolHandler> redisHandler_;
};