#include "Socket.h"
#include <cerrno>
#include <csignal>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <iostream>
const int LISTENQ=2048; 
Socket::Socket(int fd):fd_(fd){}
Socket::~Socket()
{   
    if(fd_ >= 0)
        close(fd_);
}

Socket::Socket(Socket&& other) noexcept
    : fd_(other.fd_)
{
    //  std::cout << "move construct from fd = "
    //          << other.fd_ << std::endl;
    other.fd_ = -1;
}
Socket& Socket::operator=(Socket&& other) noexcept
{
    if(this != &other)
    {
        // 先释放原来的资源
        if(fd_ >= 0)
            close(fd_);
        // 接管对方资源
        fd_ = other.fd_;
        // 对方失去所有权
        other.fd_ = -1;
    }
    return *this;
}

int Socket::getfd() const
{
    return fd_;
}

int Socket::setNonBlocking()
{
    int flag = fcntl(fd_, F_GETFL, 0);
    if(flag < 0)
        return -1;
    flag |= O_NONBLOCK;
    if(fcntl(fd_, F_SETFL, flag) < 0)
        return -1;
    return 0;
}
int Socket::setNoDelay()
{
    int enable = 1;

    if(setsockopt(fd_,
                  IPPROTO_TCP,
                  TCP_NODELAY,
                  &enable,
                  sizeof(enable)) < 0)
    {
        return -1;
    }
    return 0;
}
int Socket::setNoLinger()
{
    struct linger opt{};
    opt.l_onoff = 1;
    opt.l_linger = 0;
    if(setsockopt(fd_,
                  SOL_SOCKET,
                  SO_LINGER,
                  &opt,
                  sizeof(opt)) < 0)
    {
        return -1;
    }
    return 0;
}
int Socket::setReuseAddr()
{
    int enable = 1;
    if(setsockopt(fd_,
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  &enable,
                  sizeof(enable)) < 0)
    {
        return -1;
    }

    return 0;
}

int Socket::shutdownWrite()
{
    if(shutdown(fd_,SHUT_WR)<0)
        return -1;
    return 0;
}

Socket Socket::createListenSocket(int port)
{
    if (port <= 0 || port > 65535) return Socket(-1);
    int listen_fd;
    if((listen_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
        perror("socket");
        return Socket(-1);
    }
    struct sockaddr_in server_addr{};
    // 允许地址复用，避免服务器重启时因TIME_WAIT状态导致bind失败
    Socket listenSocket(listen_fd);
    if(listenSocket.setReuseAddr()<0){
        perror("setReuseAddr");
        return Socket(-1);
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons((unsigned short)port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenSocket.getfd(),(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
        return Socket(-1);
    if(listen(listen_fd,LISTENQ)<0)
        return Socket(-1);
    return listenSocket;
}