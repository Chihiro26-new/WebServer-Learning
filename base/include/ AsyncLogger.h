#pragma once
#include "Condition.h"
#include "Thread.h"
#include "noncopyable.h"
#include "Mutex.h"
#include "LogStream.h"
#include <cstddef>
#include <memory>
#include <vector>
#include <atomic>
class AsyncLogger:noncopyable
{
public:
    using Buffer=FixedBuffer<4*1024*1024>;
    using BufferPtr = std::unique_ptr<Buffer>;
    ~AsyncLogger();
    void stop();
    void start();
    void append(const char *logline,size_t len);
private:
    void ThreadFunc();
    Thread thread_;
    std::atomic<bool>running_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    std::vector<BufferPtr> buffers_;
    MutexLock mutex_;
    Condition cond_;
};