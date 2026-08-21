#pragma once
#include <memory>
class TcpConnection;
class ProtocolHandler
{
public:
    using TcpConnectionPtr =std::shared_ptr<TcpConnection>;
    virtual ~ProtocolHandler() = default;
    virtual void onMessage(TcpConnectionPtr conn)=0;
    virtual void onConnection(TcpConnectionPtr conn)=0;
    virtual void onClose(TcpConnectionPtr conn)=0;
};
