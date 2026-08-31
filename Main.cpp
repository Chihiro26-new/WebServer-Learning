#include "Server.h"
#include "ProtocolDispatcher.h"
#include "EventLoop.h"
#include "Util.h"
#include "WorkerPool.h"
#include <memory>
#include "RedisHandler.h"
#include "HttpHandler.h"
#include "Logger.h"
#include "AsyncLogger.h"
int main(int argc, char *argv[])
{
    handle_for_sigpipe();
    LogStream log;
    int port = 8080;
    int workerNum = 4;
    AsyncLogger logger("./WebServer.log");
    logger.start();
    std::string msg(1024 * 1024, 'A'); // 1 MB

    logger.append(msg.data(), msg.size());
    logger.append(msg.data(), msg.size());
    logger.append(msg.data(), msg.size());
    logger.append(msg.data(), msg.size());
    logger.append(msg.data(), msg.size());

    std::this_thread::sleep_for(
        std::chrono::seconds(2)
    );
    logger.stop();
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-p")
        {
            if (i + 1 >= argc)
            {
                LOG_ERROR << "Error: -p requires a port\n";
                return 1;
            }

            port = std::stoi(argv[++i]);
        }
        else if (arg == "-t")
        {
            if (i + 1 >= argc)
            {
                LOG_ERROR << "Error: -t requires a worker number\n";
                return 1;
            }

            workerNum = std::stoi(argv[++i]);
        }
        else
        {
            LOG_ERROR << "Unknown argument: " << arg << '\n';
            return 1;
        }
    }

    if (port <= 0 || port > 65535)
    {
        LOG_ERROR << "Error: invalid port\n";
        return 1;
    }

    if (workerNum <= 0)
    {
        LOG_ERROR << "Error: invalid worker number\n";
        return 1;
    }

    LOG_INFO << "Server config:"<<'\n';
    LOG_INFO << "port = " << port<<'\n';
    LOG_INFO << "workerNum = " << workerNum<<'\n';
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