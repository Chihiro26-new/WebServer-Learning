#pragma once
#include "ProtocolHandler.h"
class TcpConnection;
class RedisHandler : public ProtocolHandler
{
public:
    void onMessage(TcpConnectionPtr conn) override;


};