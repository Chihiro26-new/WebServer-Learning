#include "AsyncLogger.h"
#include "Mutex.h"
#include <assert.h>
#include "LogFile.h"
#include "Logger.h"
AsyncLogger::AsyncLogger(const std::string& logPath,int flushInterval)
    :logPath_(logPath),
    flushInterval_(flushInterval),
    thread_([this]() {
        ThreadFunc();},"LoggingThread"),
    running_(false),
    currentBuffer_(std::make_unique<Buffer>()),
    nextBuffer_(std::make_unique<Buffer>()),
    cond_(mutex_)
{

} 

AsyncLogger::~AsyncLogger()
{
    if(running_)
        stop();
}
void AsyncLogger::start()
{
    running_=true;
    thread_.start();
}
void AsyncLogger::stop()
{
    running_=false;
    cond_.notify();
    thread_.join();
}
void AsyncLogger::append(const char *logline,size_t len)
{
    LockGuard lock(mutex_);
    if(currentBuffer_->available()>=len)
    {
        currentBuffer_->append(logline, len);
    }
    else {
        LOG_INFO<<"Buffer full, notify logging thread\n";
        buffers_.push_back(std::move(currentBuffer_));
        if(nextBuffer_)
            currentBuffer_=std::move(nextBuffer_);
        else
            currentBuffer_ = std::make_unique<Buffer>();
        currentBuffer_->append(logline, len);
        cond_.notify();
    }
}
void AsyncLogger::ThreadFunc()
{
    assert(running_ == true);
    LogFile output(logPath_);
    BufferPtr newBuffer1 = std::make_unique<Buffer>();
    BufferPtr newBuffer2 = std::make_unique<Buffer>();
    std::vector<BufferPtr>buffersToWrite;
    buffersToWrite.reserve(16);
    while(running_)
    {
        {
            LockGuard lock(mutex_);
            if (buffers_.empty())
            {
                cond_.waitFor(
                    std::chrono::seconds(flushInterval_)
                );
            }
            if (currentBuffer_->length() > 0)
                buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(newBuffer1);

            buffersToWrite.swap(buffers_);
            if (!nextBuffer_)
            {
                nextBuffer_ = std::move(newBuffer2);
            }
        }
        for (const auto& buffer:buffersToWrite)
        {
            output.append(buffer->data(), buffer->length());
        }

        if (!newBuffer1 && !buffersToWrite.empty())
        {
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }
        if (!newBuffer2 && !buffersToWrite.empty())
        {
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }
        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}