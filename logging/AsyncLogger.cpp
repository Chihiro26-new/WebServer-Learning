#include "AsyncLogger.h"
#include "LogFile.h"
#include <deque>
#include <iostream>
AsyncLogger::AsyncLogger(const LogFile::LogFileOptions& options,const int flushInterval)
    :logFile_(options),
    flushInterval_(flushInterval),
    thread_([this](){
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
    {
        LockGuard lock(mutex_);
        running_=false;
    }
    cond_.notify();//唤醒线程
    thread_.join();
}
void AsyncLogger::append(const char *logline,size_t len)
{
    LockGuard lock(mutex_);
    if(!running_)
        return;
    if(currentBuffer_->available()>=len)
        currentBuffer_->append(logline, len);
    else{
        buffers_.push_back(std::move(currentBuffer_));
        if(nextBuffer_)
            currentBuffer_=std::move(nextBuffer_);
        else
            currentBuffer_=std::make_unique<Buffer>();
        currentBuffer_->append(logline,len);
        cond_.notify();
    }
}
void AsyncLogger::ThreadFunc()
{
    LogFile &output=logFile_;
    BufferPtr newBuffer1=std::make_unique<Buffer>();
    BufferPtr newBuffer2=std::make_unique<Buffer>();
    std::vector<BufferPtr>buffersToWrite;
    std::deque<BufferPtr>pendingBuffers;
    buffersToWrite.reserve(16);
    while(true)
    {
        // std::cout << "A: 开始循环\n";
        {
            LockGuard lock(mutex_);
            if(running_&&buffers_.empty())
            {
                // std::cout << "B: 准备等待\n";
                cond_.waitFor(
                    std::chrono::seconds(flushInterval_)
                );
                // std::cout << "C: 等待结束\n";
            }
            if(currentBuffer_->length()>0)
                buffers_.push_back(std::move(currentBuffer_));
            if(newBuffer1)
                currentBuffer_=std::move(newBuffer1);
            else
                currentBuffer_=std::make_unique<Buffer>();
            buffersToWrite.swap(buffers_);

            // std::cout<<"本轮搜集到:"<<buffersToWrite.size()<<"个buffer\n";
            for (const auto& buffer : buffersToWrite) {
            // std::cout << "Buffer length: "
            //         << buffer->length() << '\n';
}
            if(!nextBuffer_&&newBuffer2)
                nextBuffer_=std::move(newBuffer2);
            if(!running_&&
                buffersToWrite.empty()&&
                pendingBuffers.empty())
                break;
        }
        for(auto&buffer:buffersToWrite)
            pendingBuffers.push_back(std::move(buffer));
        buffersToWrite.clear();
        // std::cout << "D: 处理日志\n";
        while(!pendingBuffers.empty())
        {
            auto&buffer=pendingBuffers.front();
            std::cout << "[Write] pending size: "
            << pendingBuffers.size()
            << ", length: " << buffer->length() << '\n';
            if (!output.append(buffer->data(), buffer->length()))
            {
                std::cout << "[Write] 失败，保留在队首\n";
                break;
            }

        
            BufferPtr completed = std::move(pendingBuffers.front());
            pendingBuffers.pop_front();

            std::cout << "[Write] 成功，剩余: "
                        << pendingBuffers.size() << '\n';

            // 4. 回收成功写入的 Buffer
            completed->reset();

            if (!newBuffer1)
            {
                newBuffer1 = std::move(completed);
            }
            else if (!newBuffer2)
            {
                newBuffer2 = std::move(completed);
            }
        }
        output.flush();
    }
    output.flush();
}