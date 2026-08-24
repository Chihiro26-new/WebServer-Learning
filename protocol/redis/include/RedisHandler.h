#pragma once
#include "ProtocolHandler.h"
class TcpConnection;
class RedisHandler : public ProtocolHandler
{
public:
    void onMessage(TcpConnectionPtr conn) override;
    void onConnection(TcpConnectionPtr conn) override;
    void onClose(TcpConnectionPtr conn) override;
};