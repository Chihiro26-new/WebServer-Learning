#pragma once
#include "noncopyable.h"
#include <mutex>
class MutexLock:public noncopyable
{

public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();

private:
    friend class Condition;
    std::mutex&nativeHandle();

private:
    std::mutex mutex_;
};



class LockGuard : noncopyable 
{
public:
    explicit LockGuard(MutexLock &_mutex) : mutex_(_mutex)
    ,locked_(true) { mutex_.lock(); }
    ~LockGuard();
    void unlock();
private: 
    bool locked_;
    MutexLock &mutex_;
};