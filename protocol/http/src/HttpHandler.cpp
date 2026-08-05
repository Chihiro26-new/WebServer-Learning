#include "HttpHandler.h"
#include <memory>
#include "TcpConnection.h"
#include "HttpParser.h"
#include "HttpResponse.h"
HttpHandler::HttpHandler()
    :
    parser_(std::make_unique<HttpParser>())
{

}

void HttpHandler::onMessage(TcpConnectionPtr conn)
{
    auto& buffer =conn->getInputBuffer();
    HttpRequest request;
    parser_->parseRequestLine(buffer,request);
    HttpResponse response;
    response.setStatusCode(200);
    response.setBody("hello");
    conn->sendMsg(response.toString());
}