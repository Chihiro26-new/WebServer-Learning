#pragma once
#include "noncopyable.h"
#include <thread>
#include <functional>
#include <string>
// 对 std::thread 的一层简单封装。
// 负责管理线程的创建、启动、等待以及线程基本信息。
//
// 继承 noncopyable：
// Thread 对象本身不能被拷贝，避免两个 Thread 对象意外管理同一个线程。
class Thread:public noncopyable
{
public:
    // 线程执行函数类型。
    //
    // Thread 创建时传入一个无参数、无返回值的函数，
    // 在线程启动后由新线程执行。
    using ThreadFunc = std::function<void()>;

    // 构造函数。
    //
    // func：线程启动后需要执行的函数
    // name：线程名称，可选，方便调试和日志识别
    explicit Thread(
        ThreadFunc func,
        const std::string& name = ""
    );
    ~Thread();

    // 启动线程。
    //
    // 调用之后，std::thread 会创建一个新的系统线程，
    // 新线程最终执行 func_。
    void start();
    // 等待线程结束。
    //
    // 当前线程会阻塞，直到该 Thread管理的线程执行完成。
    void join();


    bool started() const;// 判断线程是否已经启动
    std::thread::id tid() const;//获取线程ID

    const std::string& name() const; // 获取线程名称

private:

    std::thread thread_;// 实际负责管理系统线程的std::thread 对象
    ThreadFunc func_; // 线程启动后真正执行的函数

    std::string name_;

    std::thread::id tid_;// 在线程真正启动后获取。
    bool started_;//标记线程是否启动
};