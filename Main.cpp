#include "EventLoop.h"
#include "Server.h"
#include "HttpHandler.h"
#include "EchoHandler.h"
#include "RedisHandler.h"
#include <iostream>
#include "Timer.h"
#include "Util.h"
#include "EventLoopThread.h"
int main(int argc,char *argv[])
{
    handle_for_sigpipe();
    // auto echoHandler = std::make_shared<EchoHandler>();
    // auto fileHandler =
    // std::make_shared<StaticFileHandler>("./www");

    // auto httpHandler = std::make_shared<HttpHandler>(fileHandler);
    // // auto redisHandler = std::make_shared<RedisHandler>();
    // EventLoop loop;
    // Server my_server(&loop,8080,httpHandler);
    // loop.loop();
    EventLoopThread thread;
    EventLoop* loop = thread.startLoop();
    std::this_thread::sleep_for(
        std::chrono::seconds(3)
    );
    return 0;
}