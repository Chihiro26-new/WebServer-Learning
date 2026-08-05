#include "HttpHandler.h"
#include "HttpContext.h"
#include "HttpResponse.h"
#include "TcpConnection.h"
#include <iostream>
HttpHandler::HttpHandler()
{
}
 HttpHandler::~HttpHandler(){};

void HttpHandler::onConnection(
    TcpConnectionPtr conn)
{
    contexts_[conn.get()] =std::make_shared<HttpContext>();
}

void HttpHandler::onClose(
    TcpConnectionPtr conn)
{
    contexts_.erase(conn.get());
}

void HttpHandler::onMessage(
    TcpConnectionPtr conn)
{
    std::cout << "HttpHandler onMessage" << std::endl;
    auto it =contexts_.find(conn.get());
    if(it == contexts_.end())
    {
        std::cout
        << "context not found"
        << std::endl;
        return;
    }
    auto context =it->second;
    auto& buffer =conn->getInputBuffer();

    bool ok =context->parseRequest(buffer);
    // std::cout 
    // << "parse finished"
    // << std::endl;
    if(!ok)
    {
        return;
    }

    if(context->gotAll())
    {
        auto& request =context->request();
        // std::cout<< request.method()<< std::endl;
        // std::cout
        //             << "path = "
        //             << request.path()
        //             << std::endl;

        HttpResponse response;

        response.setStatusCode(200);
        response.addHeader("Content-Type","text/plain");
        response.setBody("hello");
        // std::cout
        //     << "response:"
        //     << std::endl
        //     << msg
        //     << std::endl;
        conn->sendMsg(response.toString());
        context->reset();
    }
}

