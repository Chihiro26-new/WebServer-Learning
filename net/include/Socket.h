#pragma once
#include "noncopyable.h"
class Socket:noncopyable
{
public:
    static Socket createListenSocket(int port);
    explicit Socket(int fd);
    Socket(Socket&& other) noexcept;//移动构造
    Socket& operator=(Socket&& other) noexcept;
    ~Socket();
    int setNonBlocking();
    int setNoDelay();//禁用Nagle
    int setReuseAddr();
    int setNoLinger();
    int shutdownWrite();
    int getfd() const;
private:
    int fd_;
};

