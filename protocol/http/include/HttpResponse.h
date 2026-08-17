#pragma once
#include <unordered_map>
#include <string>
class Buffer;
class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    void setStatusCode(int code);
    void setBody(const std::string& body);
    std::string toString()const;
    void appendTo(Buffer& buffer) const;
    void setCloseConnection(bool on);
    bool closeConnection() const;
    void addHeader(const std::string& key,const std::string& value);
private:
    int statusCode_;//状态码
    std::unordered_map<std::string,std::string> headers_;
    std::string body_;
    bool closeConnection_; 
};

