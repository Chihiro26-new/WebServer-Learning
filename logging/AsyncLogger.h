#pragma once
#include "Condition.h"
#include "Thread.h"
#include "noncopyable.h"
#include "Mutex.h"
#include "LogStream.h"
#include <memory>
#include <vector>
#include <atomic>
class AsyncLogger:noncopyable
{
public:
    using Buffer=FixedBuffer<4*1024*1024>;
    using BufferPtr = std::unique_ptr<Buffer>;
    AsyncLogger(const std::string& logPath,int flushInterval=3);
    ~AsyncLogger();
    void stop();
    void start();
    void append(const char *logline,size_t len);
private:
    void ThreadFunc();
    const int flushInterval_;
    const std::string logPath_;
    Thread thread_;
    std::atomic<bool>running_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    std::vector<BufferPtr> buffers_;
    MutexLock mutex_;
    Condition cond_;
};