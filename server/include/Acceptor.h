#pragma once
#include "TcpConnection.h"
class Acceptor
{
    public:
        using NewConnectionCallback =std::function<void(int)>;
        Acceptor(EventLoop* loop,int port);
        ~Acceptor();
        void setNewConnectionCallback(NewConnectionCallback cb);
        void handleRead();//处理listen的fd
    private:
        NewConnectionCallback newConnectionCallback_;//回调server
        Socket acceptSocket_;   //监听socket
        Channel channel_;       //监听事件
        EventLoop* loop_;
};