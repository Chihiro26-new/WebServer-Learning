#include " AsyncLogger.h"
#include <assert.h>
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
    if(currentBuffer_->available()>len)
    {
        currentBuffer_->append(logline, len);
    }
    else {
        if(nextBuffer_)
            currentBuffer_=std::move(nextBuffer_);
        else
            currentBuffer_->reset();
        currentBuffer_->append(logline, len);
        cond_.notify();
    }
}
void AsyncLogger::ThreadFunc()
{
    assert(running_ == true);

}