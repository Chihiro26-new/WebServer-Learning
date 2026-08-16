#include "Worker.h"
#include "ProtocolHandler.h"
#include "WorkerPool.h"
WorkerPool::WorkerPool(
    int numWorkers,
    std::shared_ptr<ProtocolHandler> handler
)
    : next_(0)
    , started_(false)
{
    workers_.reserve(numWorkers);

    for (int i = 0; i < numWorkers; ++i)
    {
        workers_.push_back(
            std::make_unique<Worker>(handler)
        );
    }
}

WorkerPool::~WorkerPool()
{
    shutdown();
}

void WorkerPool::start()
{
    if (started_)
        return;

    started_ = true;

    for (auto& worker : workers_)
    {
        worker->start();
    }
}

void WorkerPool::shutdown()
{
    if (!started_)
        return;

    for (auto& worker : workers_)
    {
        worker->shutdown();
    }

    for (auto& worker : workers_)
    {
        worker->join();
    }

    started_ = false;
}

void WorkerPool::dispatch(int fd)
{
    if (workers_.empty())
    {
        return;
    }

    Worker* worker = workers_[next_].get();

    ++next_;

    if (next_ >= workers_.size())
        next_ = 0;

    worker->addConnection(fd);
}