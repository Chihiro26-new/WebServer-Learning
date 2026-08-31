#pragma once
#include "noncopyable.h"
#include <string>
#include <fstream>
class LogFile:noncopyable
{
public:
    explicit LogFile(const std::string& filename);

    ~LogFile();
    void append(const char* logline, size_t len);
    void flush();
    bool rollFile();

private:
    std::ofstream file_;
};