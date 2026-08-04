#pragma once
#include "HttpParser.h"
#include "HttpRequest.h"
#include "Buffer.h"
class HttpContext
{
public:
    HttpContext();
    // 解析buffer中的HTTP数据
    bool parseRequest(Buffer& buffer);
    // 获取解析完成的请求
    HttpRequest& request();
    // 重置状态，用于keep-alive下处理下一次请求
    void reset();
private:
    HttpParser parser_;
    HttpRequest request_;
};