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
    auto [it, inserted] =
    contexts_.emplace(
        conn.get(),
        std::make_shared<HttpContext>()
    );
    auto context =it->second;
    auto& buffer =conn->getInputBuffer();
    while(buffer.readableBytes()>0)
    {
        // std::cout
        // << "before parse, remain="
        // << buffer.readableBytes()
        // << std::endl;
        bool ok =context->parseRequest(buffer);
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

            auto& outputBuffer =
            conn->getOutputBuffer();
            response.appendTo(outputBuffer);
            conn->enableOutput();
            context->reset();

        }
        else
            break;
    }
}

