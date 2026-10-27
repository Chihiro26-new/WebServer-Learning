#pragma once
#include "Condition.h"
#include "Thread.h"
#include "noncopyable.h"
#include "Mutex.h"
#include "LogStream.h"
#include <memory>
#include <vector>
#include <atomic>

/**
 * @brief 异步日志系统
 *
 * 采用单独的后台日志线程，将业务线程产生的日志数据
 * 暂存在缓冲区中，再由后台线程批量写入日志文件。
 *
 * 主要设计：
 * 1. 前台线程调用 append()，将日志写入 currentBuffer_。
 * 2. 当前缓冲区空间不足时，将其移入 buffers_ 队列，
 *    并切换到 nextBuffer_ 或新分配的缓冲区。
 * 3. 后台线程通过条件变量等待数据或定时唤醒，
 *    批量取出缓冲区并写入日志文件。
 * 4. 通过双缓冲及缓冲区复用，减少频繁内存分配，
 *    降低前台线程写日志时的阻塞开销。
 *
 * 线程安全：
 * 通过 MutexLock 保护共享缓冲区及缓冲区队列，
 * 通过 Condition 协调前后台线程，
 * 通过 atomic<bool> running_ 表示运行状态。
 **/

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