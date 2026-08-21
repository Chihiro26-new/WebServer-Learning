#include "Worker.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "ProtocolDispatcher.h"
Worker::Worker(
    std::shared_ptr<ProtocolDispatcher> dispatcher
)
    :
    loopThread_(),
    loop_(nullptr),
    dispatcher_(std::move(dispatcher))
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
            auto conn =
                std::make_shared<TcpConnection>(
                    loop_,
                    fd
                );

            auto dispatcher = dispatcher_;
            // 注入协议判断能力
            conn->setProtocolFactory(
                [dispatcher = dispatcher_]
                (Buffer& buffer)
                {
                    return dispatcher->dispatch(buffer);
                }
            );

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