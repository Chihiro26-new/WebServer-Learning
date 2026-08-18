#include "Mutex.h"
#include <mutex>

MutexLock::MutexLock() = default;
MutexLock::~MutexLock() = default;
void MutexLock::lock()
{
    mutex_.lock();
}
void MutexLock::unlock()
{
    mutex_.unlock();
}
std::mutex&MutexLock::nativeHandle()
{
    return mutex_;
}

LockGuard::~LockGuard()
{
    if(locked_)
        mutex_.unlock();
}


void LockGuard::unlock()
{
    if(locked_)
    {
        locked_=false;
        mutex_.unlock();
    }
}

