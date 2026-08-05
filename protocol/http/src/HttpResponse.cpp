#include "HttpResponse.h"
#include <sstream>

HttpResponse::HttpResponse()
    :statusCode_(200)
{

}
HttpResponse::~HttpResponse(){}
void HttpResponse::setStatusCode(int code)
{
    statusCode_ = code;
}

void HttpResponse::setBody(const std::string& body)
{
    body_ = body;
}

void HttpResponse::addHeader(const std::string& key,const std::string& value)
{
    headers_[key]=value;
}

std::string HttpResponse::toString() const
{
    std::stringstream ss;
    // 状态行
    ss<<"HTTP/1.1 "
      <<statusCode_
      <<" OK\r\n";

    // header
    for(auto& [key,value]:headers_)
    {
        ss<<key
          <<": "
          <<value
          <<"\r\n";
    }

    ss<<"Content-Length: "
      <<body_.size()
      <<"\r\n";


    ss<<"\r\n";
    // body
    ss<<body_;

    return ss.str();
}