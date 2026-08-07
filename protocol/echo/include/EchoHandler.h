#pragma once
#include "ProtocolHandler.h"
class TcpConnectionPtr;
class EchoHandler : public ProtocolHandler
{
public:
    void onMessage(TcpConnectionPtr conn) override;
    void onConnection(TcpConnectionPtr conn) override;
    void onClose(TcpConnectionPtr conn) override;
};