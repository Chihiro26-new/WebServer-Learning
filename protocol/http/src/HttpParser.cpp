#include "HttpParser.h"
#include "Buffer.h"
#include <string>
//#include <iostream>
ParseResult HttpParser::parseRequestLine(
        Buffer& buffer,
        HttpRequest& request)
{
    std::string line;
    // Buffer里面没有完整一行
    if(!getLine(buffer,line))
    {
        return ParseResult::NeedMoreData;
    }

    if(splitRequestLine(line,request))
    {
        return ParseResult::Done;
    }

    return ParseResult::Error;
}

ParseResult HttpParser::parseHeader(
        Buffer& buffer,
        HttpRequest& request)
{
    std::string line;

    if(!getLine(buffer,line))
    {
        return ParseResult::NeedMoreData;
    }
    /*
        HTTP:
        
        Header1\r\n
        Header2\r\n
        \r\n

        空行表示header结束
    */
    if(line.empty())
    {
        return ParseResult::Done;
    }

    if(splitHeader(line,request))
    {
        // 一个header解析完成
        // 但是header阶段没有结束
        return ParseResult::Continue;
    }


    return ParseResult::Error;
}

ParseResult HttpParser::parseBody(
        Buffer& buffer,
        HttpRequest& request)
{
    auto it = request.headers().find("Content-Length");
    size_t contentLength=0;
    if (it != request.headers().end())
    {
        try
        {
            contentLength = std::stoull(it->second);
        }
        catch (...)
        {
            return ParseResult::Error;
        }
    }
    // std::cout << "Content-Length = "
    //       << contentLength
    //       << ", readable = "
    //       << buffer.readableBytes()
    //       << std::endl;
    if (buffer.readableBytes() < contentLength)
    {
        return ParseResult::NeedMoreData;
    }
    std::string body(
        buffer.peek(),
        contentLength
    );
    // std::cout << "Body = " << body << std::endl;
    buffer.retrieve(contentLength);
    request.setBody(std::move(body));
    return ParseResult::Done;
}


bool HttpParser::getLine(
        Buffer& buffer,
        std::string& line)
{
    const char* start =
        buffer.peek();
    size_t len =
        buffer.readableBytes();

    for(size_t i=0;i+1<len;i++)
    {
        if(start[i]=='\r' &&
           start[i+1]=='\n')
        {

            line.assign(start,i);
            // 消费掉这一行+CRLF
            buffer.retrieve(i+2);
            return true;
        }
    }
    return false;
}


bool HttpParser::splitRequestLine(
    const std::string& line,
    HttpRequest& request)
{
    // GET /index.html HTTP/1.1
    //
    //        p1       p2
    //        ↓        ↓
    // GET /index.html HTTP/1.1

    const size_t p1 = line.find(' ');
    if (p1 == std::string::npos || p1 == 0)
    {
        return false;
    }

    const size_t p2 = line.find(' ', p1 + 1);
    if (p2 == std::string::npos || p2 == p1 + 1)
    {
        return false;
    }

    // version 不能为空
    if (p2 + 1 >= line.size())
    {
        return false;
    }
    request.setMethod(
        line.substr(0, p1)
    );

    request.setPath(
        line.substr(
            p1 + 1,
            p2 - p1 - 1
        )
    );

    request.setVersion(
        line.substr(p2 + 1)
    );
    //   std::cout
    // << request.getMethod()
    // << " "
    // << request.getPath()
    // << std::endl;
    return true;
}


bool HttpParser::splitHeader(
    const std::string& line,
    HttpRequest& request)
{
    auto pos = line.find(':');

    if (pos == std::string::npos)
    {
        return false;
    }

    size_t valueStart = pos + 1;

    // 跳过 value 前面的空格
    while (valueStart < line.size() &&
           line[valueStart] == ' ')
    {
        ++valueStart;
    }

    std::string key = line.substr(0, pos);
    std::string value = line.substr(valueStart);

    request.addHeader(key, value);

    return true;
}