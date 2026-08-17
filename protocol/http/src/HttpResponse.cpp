#include "HttpResponse.h"
#include "Buffer.h"
HttpResponse::HttpResponse()
    :statusCode_(200),closeConnection_(false)
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
void HttpResponse::setCloseConnection(bool on)
{
    closeConnection_ = on;
}

bool HttpResponse::closeConnection() const
{
    return closeConnection_;
}

void HttpResponse::appendTo(Buffer& buffer) const
{
    buffer.append("HTTP/1.1 ");

    buffer.append(
        std::to_string(statusCode_)
    );

    buffer.append(" OK\r\n");
    for (const auto& [key, value] : headers_)
    {
        buffer.append(key);
        buffer.append(": ");
        buffer.append(value);
        buffer.append("\r\n");
    }
    buffer.append("Content-Length: ");

    buffer.append(
        std::to_string(body_.size())
    );

    buffer.append("\r\n");
    buffer.append("\r\n");

    buffer.append(body_);
}
std::string HttpResponse::toString() const
{
    std::string result;

    result.reserve(256 + body_.size());

    result += "HTTP/1.1 ";
    result += std::to_string(statusCode_);
    result += " OK\r\n";

    for (const auto& [key, value] : headers_)
    {
        result += key;
        result += ": ";
        result += value;
        result += "\r\n";
    }

    result += "Content-Length: ";
    result += std::to_string(body_.size());
    result += "\r\n";

    result += "\r\n";

    result += body_;

    return result;
}