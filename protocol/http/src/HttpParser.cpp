#include "HttpParser.h"
#include <sstream>

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

    /*
        简化版本：
        不考虑Content-Length
        直接把剩余数据当body
    */
    if(buffer.readableBytes()==0)
    {
        return ParseResult::Done;
    }
    std::string body =
        buffer.retrieveAllAsString();
    request.setBody(body);
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

    std::istringstream iss(line);

    std::string method;
    std::string path;
    std::string version;

    iss >> method
        >> path
        >> version;
    if(method.empty() ||
       path.empty() ||
       version.empty())
    {
        return false;
    }

    request.setMethod(method);
    request.setPath(path);
    request.setVersion(version);

    return true;
}


bool HttpParser::splitHeader(
        const std::string& line,
        HttpRequest& request)
{

    auto pos =
        line.find(':');
    if(pos==std::string::npos)
    {
        return false;
    }

    std::string key =
        line.substr(0,pos);
    std::string value =
        line.substr(pos+1);

    // 去掉value前面的空格
    while(!value.empty()
          && value[0]==' ')
    {
        value.erase(value.begin());
    }

    request.addHeader(
        key,
        value);
    return true;
}