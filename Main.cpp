#include "Server.h"
#include "HttpHandler.h"
#include "EchoHandler.h"
// #include "RedisHandler.h"
#include <iostream>
#include <memory>
#include "EventLoop.h"
#include "Util.h"
#include "WorkerPool.h"
// #include "Buffer.h"
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

    std::cout << "Server config:\n"
              << "  port       = " << port << '\n'
              << "  workerNum  = " << workerNum << '\n';

    auto fileHandler =
        std::make_shared<StaticFileHandler>("./www");

   

    auto echoHandler =
        std::make_shared<EchoHandler>();

    EventLoop mainLoop;

    WorkerPool workerPool(
        workerNum,
        [fileHandler]()
        {
            return std::make_shared<HttpHandler>(
                fileHandler
            );
        }
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