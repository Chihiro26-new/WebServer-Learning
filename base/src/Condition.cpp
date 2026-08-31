#include "Condition.h"
#include <chrono>
#include <mutex>
Condition::Condition(MutexLock&mutex)
    :mutex_(mutex)
{

}
Condition::~Condition(){}
void Condition::notify()
{
    cond_.notify_one();
}
void Condition::notifyAll()
{
    cond_.notify_all();
}
void Condition::wait()
{
    std::unique_lock<std::mutex>lock(
        mutex_.nativeHandle(),
        std::adopt_lock
    );
    cond_.wait(lock);
    lock.release();
}
void Condition::waitFor(std::chrono::seconds timeout)
{
    std::unique_lock<std::mutex>lock(
        mutex_.nativeHandle(),
        std::adopt_lock
    );
    cond_.wait_for(lock,timeout);
    lock.release();
}