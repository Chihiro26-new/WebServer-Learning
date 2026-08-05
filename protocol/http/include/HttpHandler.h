#pragma once
#include "HttpContext.h"
#include "ProtocolHandler.h"
#include <unordered_map>

class HttpContext;
class HttpHandler : public ProtocolHandler
{
public:
    HttpHandler();
    ~HttpHandler();
    void onMessage(TcpConnectionPtr conn) override;//处理数据
    void onConnection(TcpConnectionPtr conn) override;//准备HTTP状态机
    void onClose(TcpConnectionPtr conn) override;//处理关闭

private:
  std::unordered_map<TcpConnection*,std::shared_ptr<HttpContext>> contexts_;

};