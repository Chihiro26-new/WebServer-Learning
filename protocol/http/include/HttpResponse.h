#pragma once
#include <map>
#include <string>
class HttpResponse
{
public:
    void setStatusCode(int code);
    void setBody(const std::string& body);
    std::string toString();

private:

    int statusCode_;
    std::map<std::string,std::string> headers_;
    std::string body_;
};

