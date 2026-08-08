#pragma once
#include "HttpContext.h"
#include "ProtocolHandler.h"
#include "StaticFileHandler.h"
#include <unordered_map>
class StaticFileHandler;
class HttpContext;
class HttpHandler : public ProtocolHandler
{
public:
    ~HttpHandler();
    explicit HttpHandler(std::shared_ptr<StaticFileHandler> handler);
    void onMessage(TcpConnectionPtr conn) override;//处理数据
    void onConnection(TcpConnectionPtr conn) override;//准备HTTP状态机
    void onClose(TcpConnectionPtr conn) override;//处理关闭


private:
  std::unordered_map<TcpConnection*,std::shared_ptr<HttpContext>> contexts_;
  std::shared_ptr<StaticFileHandler> fileHandler_;
};