#include "HttpHandler.h"
#include "HttpContext.h"
#include "HttpResponse.h"
#include "TcpConnection.h"
#include <iostream>
#include "StaticFileHandler.h"


HttpHandler::~HttpHandler(){};
HttpHandler::HttpHandler(std::shared_ptr<StaticFileHandler> handler)
        : fileHandler_(std::move(handler))
{

}


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
    // std::cout << "HttpHandler onMessage" << std::endl;
    auto it =contexts_.find(conn.get());
    if(it == contexts_.end())
    {
        std::cout<< "context not found"<< std::endl;
        return;
    }
    auto context =it->second;
    auto& buffer =conn->getInputBuffer();
    while(buffer.readableBytes()>0)
    {
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
            HttpResponse response;
              // 静态文件交给StaticFileHandler
            fileHandler_->handle(
                request,
                response
            );

            if(!request.keepAlive())
            {
                std::cout 
                    << "close after write"
                    << std::endl;

                response.setCloseConnection(true);

                conn->startDisconnect();
            }

            conn->sendMsg(
                response.toString()
            );
            context->reset();
            }
        else
            break;
    }
}

