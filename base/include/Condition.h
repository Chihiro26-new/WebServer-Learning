#pragma once
#include "noncopyable.h"
#include "Mutex.h"
#include <condition_variable>
class Condition : noncopyable
{   
public:
    explicit Condition(MutexLock&mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();
private:    

    MutexLock&mutex_;
    std::condition_variable cond_;
};