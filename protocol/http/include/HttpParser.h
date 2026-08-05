#pragma once
#include "HttpRequest.h"
class Buffer;
enum class ParseResult
{
    Continue,       //当前解析成功，还没结束
    Done,           //当前阶段完成
    NeedMoreData,   //等待更多TCP数据
    Error           //协议错误
};
class HttpParser
{
public:
    ParseResult parseRequestLine(Buffer& buffer,HttpRequest&request);
    ParseResult parseHeader(Buffer& buffer,HttpRequest& request);
    ParseResult parseBody(Buffer& buffer,HttpRequest& request);
private:
    bool getLine(Buffer& buffer,std::string& line);// 获取一行数据
    bool splitRequestLine(const std::string& line,HttpRequest& request);// 解析请求行
    bool splitHeader(const std::string& line,HttpRequest& request);// 解析请求头
};