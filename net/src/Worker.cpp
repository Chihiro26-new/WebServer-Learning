#include "Worker.h"
#include "EventLoop.h"
#include "TcpConnection.h"
// #include <iostream>
Worker::Worker(
    std::shared_ptr<ProtocolHandler> handler
)
    : loopThread_()
    , loop_(nullptr)
    , handler_(std::move(handler))
{
}
void Worker::start()
{
    loop_ = loopThread_.startLoop();
}
void Worker::join()
{
    loopThread_.join();
}
void Worker::shutdown()
{
    if (loop_ != nullptr)
    {
        loop_->quit();
    }
}
void Worker::addConnection(int fd)
{
    loop_->runInLoop(
        [this, fd]()
        {
            // std::cout
            //     << "[Worker] add fd = "
            //     << fd
            //     << ", thread = "
            //     << std::this_thread::get_id()
            //     << std::endl;
            auto conn =
                std::make_shared<TcpConnection>(
                    loop_,
                    fd
                );
            conn->setProtocolHandler(handler_);
            conn->setCloseCallback(
                [this](int fd)
                {
                    removeConnection(fd);
                }
            );
            connections_[fd] = conn;
            conn->connectEstablished();
        }
    );
}

void Worker::removeConnection(int fd)
{
    connections_.erase(fd);
}