#pragma once
#include "noncopyable.h"
#include <memory>
#include <vector>
class Worker;
class ProtocolHandler;
class WorkerPool : noncopyable
{
public:
    WorkerPool(
        int numWorkers,
        std::shared_ptr<ProtocolHandler> handler
    );

    ~WorkerPool();

    // 启动所有 Worker
    void start();
    void shutdown();

    // 将 fd 分发给下一个 Worker
    void dispatch(int fd);

private:
    // 所有 Worker
    std::vector<
        std::unique_ptr<Worker>
    > workers_;

    // 当前轮询到的 Worker
    size_t next_;
    bool started_;
};
