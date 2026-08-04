#pragma once
#include "ProtocolHandler.h"
class TcpConnectionPtr;
class EchoHandler : public ProtocolHandler
{
public:
    void onMessage(TcpConnectionPtr conn) override;
};