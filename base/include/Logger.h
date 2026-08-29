#pragma once
#include "LogStream.h"
#include <cstring>
class SourceFile
{
public:
    template<size_t N>
    SourceFile(const char (&arr)[N])
        :data_(arr),size_(N-1)
    {
        const char *slash =strrchr(data_,'/');
        if (slash)
        {
            data_ = slash + 1;
            size_ -= static_cast<int>(data_ - arr);
        }
    }
    explicit SourceFile(const char *filename)
        :data_(filename)
    {
        const char* slash = strrchr(filename, '/');
        if (slash)
        {
            data_ = slash + 1;
        }
        size_ = static_cast<int>(strlen(data_));
    }
    const char* data_;
    int size_;
};
class Logger
{
public:
    enum class Level
    {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
    Logger(
        const SourceFile& file,
        int line,
        Level level
    );

    ~Logger();
    
    LogStream& stream();
    static const char* levelToString(Logger::Level level);
    void formatTime();

    using OutputFunc = void (*)(const char*, size_t);
    using FlushFunc=void(*)();
    static void setOutput(OutputFunc);//设置输出位置
    static void setFlush(FlushFunc);//设置flush时间
    static void setLogLevel(Level level);
    static bool shouldLog(Level level);
private:
    SourceFile file_;
    int line_;
    Level level_;
    LogStream stream_;
    static OutputFunc output_;
    static Level logLevel_;
    static FlushFunc flush_;
};
#define LOG_INFO \
    if (!Logger::shouldLog(Logger::Level::INFO));\
    else Logger(__FILE__, __LINE__, Logger::Level::INFO).stream()

#define LOG_WARN\
    if (!Logger::shouldLog(Logger::Level::WARN));\
    else Logger(__FILE__, __LINE__, Logger::Level::WARN).stream()

#define LOG_DEBUG\
    if (!Logger::shouldLog(Logger::Level::DEBUG));\
    else Logger(__FILE__, __LINE__, Logger::Level::DEBUG).stream()

#define LOG_ERROR\
    if (!Logger::shouldLog(Logger::Level::ERROR));\
    else Logger(__FILE__, __LINE__, Logger::Level::ERROR).stream()