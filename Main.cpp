#include "EventLoop.h"
#include "Server.h"
int main(int argc,char *argv[])
{
    EventLoop loop;
    Server my_server(&loop,8080);
    loop.loop();
    return 0;
}