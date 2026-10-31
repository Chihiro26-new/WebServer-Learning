#pragma once
#include "noncopyable.h"
#include <cstddef>
#include <fstream>
class LogFile:noncopyable
{
public:
    struct LogFileOptions
    {
        size_t rollSize = 100*1024 * 1024;
        size_t maxFiles = 5;
        std::string directory="./logs";
        std::string basename="WerServer_";
    };
    LogFile();//委托构造
    explicit LogFile(const LogFileOptions&options);

    ~LogFile();
    bool append(const char* logline, size_t len);
    bool flush();
    bool rollFile();
    void creatDirctory();
    bool openFile();

    std::string getLogFilename()const;
private:
    LogFileOptions options_;
    std::ofstream file_;

    size_t writtenBytes_=0;
    size_t currentFileIndex_ = 0;
    bool loadState();
    bool saveState();
};