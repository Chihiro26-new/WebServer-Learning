#pragma once
#include <string>

struct ReadResult
{
    ssize_t nread;   // 本次读取的字节数
    bool closed;     // 对端是否关闭
    bool error;      // 是否发生错误
};
ReadResult readn(int fd, std::string& buffer);
ssize_t readNBytes(int fd,std::string&inBuffer);//固定大小buffer
ssize_t readNBytes(int fd,void*buffer,size_t n);
ssize_t writeNBytes(int fd,std::string&outBuffer);
ssize_t writeNBytes(int fd,const void *buff, size_t n);
void handle_for_sigpipe();
