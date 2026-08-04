#pragma once
#include "Buffer.h"
#include "HttpRequest.h"
class HttpParser
{
    public:
        bool parse(Buffer&buffer,HttpRequest&request);
    private:
        bool parseRequestLine(std::string&line,HttpRequest&request);
        bool parseHeader(std::string& line,HttpRequest& request);
    
};