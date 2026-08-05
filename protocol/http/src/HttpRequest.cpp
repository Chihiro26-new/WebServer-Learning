#include "HttpRequest.h"
HttpRequest::HttpRequest(){}
HttpRequest::~HttpRequest(){}

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

