#include "EventLoop.h"
#include "Server.h"
#include "HttpHandler.h"
#include "EchoHandler.h"
#include "RedisHandler.h"
int main(int argc,char *argv[])
{
    auto echoHandler = std::make_shared<EchoHandler>();
    EventLoop loop;
    Server my_server(&loop,8080,echoHandler);
    loop.loop();
    return 0;
}