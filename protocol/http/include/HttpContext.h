#pragma once
#include "HttpParser.h"
#include "HttpRequest.h"
class Buffer;
// 管理一次HTTP请求解析状态
class HttpContext
{
public:
    enum State
    {
        RequestLine,// 解析请求行
        Headers,// 解析请求头
        Body, // 解析消息体
        Finish// 解析完成
    };

public:
    HttpContext();
    ~HttpContext();
    bool parseRequest(Buffer& buffer);//调度解析器
    bool gotAll()const;// 判断请求是否解析完成
    const HttpRequest& request() const;// 获取解析完成的请求
    void reset();// 重置状态，用于keep-alive下处理下一次请求
private:
    State state_;// 当前解析状态
    HttpParser parser_; // HTTP解析器
    HttpRequest request_; // 保存解析结果
};