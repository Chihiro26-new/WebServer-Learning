#pragma once
#include "LogStream.h"
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
        const char* file,
        int line,
        Level level
    );

    ~Logger();

    LogStream& stream();

private:

    const char* file_;
    int line_;
    Level level_;
    LogStream stream_;
};
