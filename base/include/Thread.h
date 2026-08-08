#pragma once
#include "noncopyable.h"
#include <thread>
#include <functional>
#include <string>
class Thread:public noncopyable
{
public:

    using ThreadFunc = std::function<void()>;


    explicit Thread(
        ThreadFunc func,
        const std::string& name = ""
    );
    ~Thread();

    void start();

    void join();


    bool started() const;
    std::thread::id tid() const;

    const std::string& name() const;

private:

    std::thread thread_;

    ThreadFunc func_;

    std::string name_;
    bool joined_;

    std::thread::id tid_;
    bool started_;
};