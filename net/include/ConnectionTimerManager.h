#pragma once
#include <memory>
#include <array>
#include "TimerId.h"
#include "Timer.h"
class TcpConnection;
class EventLoop;

/*
 * TCP连接超时类型
 *
 * 每一种超时类型对应一个TimerId，
 * 用于独立管理不同生命周期事件。
 */
enum class TimeoutType
{
    None,       // 无效类型，占位
    Idle,       // 空闲超时（长时间没有读写）
    Writing,    // 写事件超时
    Close,      // 关闭连接等待超时
    Heartbeat,  // 心跳检测超时
    Max         // 用于计算数组大小
};

/*
 * ConnectionTimerManager
 *
 * TcpConnection的定时器管理器。
 *
 * 负责：
 *  1. 创建连接相关定时任务
 *  2. 保存各类定时器ID
 *  3. 取消指定类型定时器
 *  4. 管理连接生命周期相关超时逻辑
 *
 * TcpConnection本身只关注连接状态和IO处理，
 * 定时器细节由该类封装。
 */
class ConnectionTimerManager
{
public:
        explicit ConnectionTimerManager(EventLoop* loop);
        void setConnection(
                std::weak_ptr<TcpConnection> conn
            );

        void startIdleTimer();
        void refreshIdleTimer();
        void startCloseTimer();
        void startHeartbeatTimer();

        void cancel(TimeoutType type);

        void cancelAll();
private:
    TimerId createTimer(
        TimeoutType type,
        TimePoint expire,
        Timer::Callback cb
    );

private:
    EventLoop* loop_;
    std::weak_ptr<TcpConnection> conn_;
    std::array<
        TimerId,
        static_cast<size_t>(TimeoutType::Max)
    > timers_;
};