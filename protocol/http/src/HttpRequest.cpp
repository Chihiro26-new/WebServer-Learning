#include "HttpRequest.h"
HttpRequest::HttpRequest(){}
HttpRequest::~HttpRequest(){}
#include <iostream>
void HttpRequest::reset()
{
    method_.clear();
    path_.clear();
    version_.clear();
    headers_.clear();
    body_.clear();
}

void HttpRequest::setMethod(const std::string&method)
{
    method_=method;
}
void HttpRequest::setPath(const std::string& path)
{
    path_ = path;
}
void HttpRequest::setVersion(const std::string&version)
{
    version_=version;
}

void HttpRequest::setBody(const std::string&body)
{
    body_=body;
}
void HttpRequest::addHeader(const std::string& key,const std::string& value)
{
    //  std::cout 
    //     << "header: "
    //     << key 
    //     << "="
    //     << value
    //     << std::endl;
    headers_[key] = value;
}
const std::string&HttpRequest::method() const
{
    return method_;
}
const std::string&HttpRequest::path()const
{
    return path_;
}
const std::string& HttpRequest::version() const
{
    return version_;
}
const std::string& HttpRequest::body() const
{
    return body_;
}
const std::string& HttpRequest::getHeader(const std::string&key)const
{   
    static const std::string empty;
    auto it=headers_.find(key);
    if(it != headers_.end())
        return it->second;
    return empty;
}

bool HttpRequest::keepAlive()const
{
    auto it = headers_.find("Connection");
    if(it != headers_.end())
    {
        if(it->second == "close")
            return false;
        if(it->second == "keep-alive")
            return true;
    }
    if(version_ == "HTTP/1.1")// HTTP/1.1 默认保持
    {
        return true;
    }
    return false;// HTTP/1.0 默认关闭
}
bool HttpRequest::hasBody() const
{
    auto it = headers_.find("Content-Length");
    if(it == headers_.end())
    {
        return false;
    }
    return std::stoi(it->second) > 0;
}
