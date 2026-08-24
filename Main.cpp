#include "Server.h"
#include "ProtocolDispatcher.h"
#include <iostream>
#include "EventLoop.h"
#include "Util.h"
#include "WorkerPool.h"
#include <memory>
#include "RedisHandler.h"
#include "HttpHandler.h"
int main(int argc, char *argv[])
{
    handle_for_sigpipe();

    int port = 8080;
    int workerNum = 4;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-p")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: -p requires a port\n";
                return 1;
            }

            port = std::stoi(argv[++i]);
        }
        else if (arg == "-t")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: -t requires a worker number\n";
                return 1;
            }

            workerNum = std::stoi(argv[++i]);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << '\n';
            return 1;
        }
    }

    if (port <= 0 || port > 65535)
    {
        std::cerr << "Error: invalid port\n";
        return 1;
    }

    if (workerNum <= 0)
    {
        std::cerr << "Error: invalid worker number\n";
        return 1;
    }

    std::cout << "  Server config:\n"
              << "  port       = " << port << '\n'
              << "  workerNum  = " << workerNum << '\n';

    auto staticHandler=
        std::make_shared<StaticFileHandler>(
            "./www"
        );
    auto httpHandler=
        std::make_shared<HttpHandler>(
            staticHandler
        );
    auto redisHandler=
        std::make_shared<RedisHandler>(
        );
    auto dispatcher=
        std::make_shared<MultiProtocolDispatcher>(
            httpHandler,
            redisHandler
        );
    EventLoop mainLoop;
    WorkerPool workerPool(
        workerNum,
        dispatcher
    );
  
    workerPool.start();
    Server server(
        &mainLoop,
        &workerPool,
        port
    );
    mainLoop.loop();
    return 0;
}