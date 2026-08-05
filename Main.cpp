#include "EventLoop.h"
#include "Server.h"
#include "HttpHandler.h"
#include "EchoHandler.h"
#include "RedisHandler.h"
#include <iostream>
#include "Timer.h"
int main(int argc,char *argv[])
{
    // auto echoHandler = std::make_shared<EchoHandler>();
    auto httpHandler = std::make_shared<HttpHandler>();
    // auto redisHandler = std::make_shared<RedisHandler>();
    EventLoop loop;
    // loop.addTimer(
    //     Clock::now() + std::chrono::seconds(10),
    //     []()
    //     {
    //         std::cout<<"Timer1 callback!\n";
    //     }
    // );

    loop.addTimer(
        Clock::now() + std::chrono::seconds(3),
        []()
        {
            std::cout<<"Timer2 callback!\n";
        }
    );
    Server my_server(&loop,8080,httpHandler);
    loop.loop();
    return 0;
}