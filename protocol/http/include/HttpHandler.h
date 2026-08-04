#pragma once
#include "ProtocolHandler.h"
class HttpParser;

class HttpHandler : public ProtocolHandler
{
public:

    HttpHandler();
    void onMessage(
        TcpConnectionPtr conn
    ) override;


private:
    std::unique_ptr<HttpParser> parser_;
};