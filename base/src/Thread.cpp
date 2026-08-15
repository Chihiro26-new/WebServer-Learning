#include "Thread.h"
#include <stdexcept>
Thread::Thread(
    ThreadFunc func,
    const std::string& name
)
    : thread_()
    , func_(std::move(func))
    , name_(name)
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

    thread_ = std::thread(
        [this]()
        {
            tid_ = std::this_thread::get_id();
            func_();
        }
    );
    started_ = true;
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

std::thread::id Thread::tid() const
{
    return tid_;
}

const std::string& Thread::name() const
{
    return name_;
}