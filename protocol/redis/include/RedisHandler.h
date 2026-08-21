#pragma once
#include "ProtocolHandler.h"
class TcpConnection;
class RedisHandler : public ProtocolHandler
{
public:
    void onMessage(TcpConnectionPtr conn) override;
    void onConnection(TcpConnectionPtr conn) override;//准备HTTP状态机
    void onClose(TcpConnectionPtr conn) override;//处理关闭
};