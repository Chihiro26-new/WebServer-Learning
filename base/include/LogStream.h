#pragma once
#include "noncopyable.h"
#include <cstddef> 
#include <string.h>
#include <string>
template<int SIZE>
class FixedBuffer
{
public:
    FixedBuffer();
    void append(const char *data,size_t len);
    const char *data()const;
    size_t length()const;
    size_t available()const;
    void reset();
    char *current();
    void add(size_t len);
private:
    char data_[SIZE];
    size_t writeIndex_;
};

class LogStream:noncopyable
{
public:
    static constexpr size_t kMaxNumericSize = 32;
    static constexpr size_t kBufferSize = 4096;
    using Buffer=FixedBuffer<kBufferSize>;
    LogStream&operator<<(bool v)
    {
        buffer_.append(v?"1":"0",1);
        return *this;
    }

    LogStream&operator<<(short);
    LogStream&operator<<(unsigned short);
    LogStream&operator<<(int);
    LogStream&operator<<(unsigned int);
    LogStream&operator<<(long);
    LogStream&operator<<(unsigned long);
    LogStream&operator<<(long long);
    LogStream&operator<<(unsigned long long);
    LogStream&operator<<(const void*);
    LogStream&operator<<(double);
    LogStream&operator<<(long double);
    LogStream&operator<<(char v)
    {
        buffer_.append(&v,1);
        return *this;
    }
    LogStream&operator<<(const char *str)
    {
        if(str)
            buffer_.append(str,strlen(str));
        else 
            buffer_.append("(null)",6);
        return *this;
    }
    LogStream&operator<<(const std::string&string)
    {
        buffer_.append(string.c_str(),string.size());
        return *this;
    }
    void append(const char* data, size_t len) { buffer_.append(data, len);}
    const Buffer&getBuffer()const;
    void resetBuffer();
private:
    template <typename T>
    void formatInteger(T);
    Buffer buffer_;
};