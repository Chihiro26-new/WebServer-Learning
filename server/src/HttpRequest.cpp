#include "HttpRequest.h"
void HttpRequest::reset()
{
    method_.clear();
    path_.clear();
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
std::string HttpRequest::getHeader(const std::string&key)
{   
    auto it = headers_.find(key);
    if(it != headers_.end())
        return it->second;
    return "";
}