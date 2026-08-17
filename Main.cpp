#include "Server.h"
#include "HttpHandler.h"
#include "EchoHandler.h"
// #include "RedisHandler.h"
// #include <iostream>
#include <memory>
#include "EventLoop.h"
#include "Util.h"
#include "WorkerPool.h"
#include "Worker.h"
int main(int argc, char *argv[])
{
    handle_for_sigpipe();

    auto fileHandler =
        std::make_shared<StaticFileHandler>("./www");

    auto httpHandler =
        std::make_shared<HttpHandler>(fileHandler);
    auto echoHandler=std::make_shared<EchoHandler>();
    EventLoop mainLoop;
    WorkerPool workerPool(
        4,
        httpHandler
    );
    workerPool.start();
    Server server(
        &mainLoop,
        &workerPool,
        8080
    );
    mainLoop.loop();

    return 0;
}