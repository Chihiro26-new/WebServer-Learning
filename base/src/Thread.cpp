#include "Thread.h"
#include <stdexcept>
Thread::Thread(
    ThreadFunc func,
    const std::string& name
)
    : thread_()
    , func_(std::move(func))
    , name_(name)
    , joined_(false)
    , tid_()
    , started_(false)
{

}

Thread::~Thread()
{
    if(thread_.joinable())
    {
        thread_.join();
    }
}
void Thread::start()
{
    if(started_)
    {
        throw std::runtime_error(
            "Thread already started"
        );
    }

    started_ = true;

    thread_ = std::thread(
        [this]()
        {
            func_();
        }
    );
}
void Thread::join()
{
    if(thread_.joinable())
    {
        thread_.join();
    }
}



bool Thread::started() const
{
    return started_;
}