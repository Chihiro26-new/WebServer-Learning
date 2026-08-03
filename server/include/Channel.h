#pragma once
#include <cstdint>
#include <functional>

class EventLoop;
class Channel
{
    using CallBack = std::function<void()>;
    public:
        // 创建一个 Channel
        // loop: 当前 Channel 所属的事件循环
        Channel(EventLoop* loop, int fd);
        Channel(EventLoop*loop);
        // 获取当前 Channel 希望监听的事件
        // EPOLLIN  : 可读事件
        // EPOLLOUT : 可写事件
        uint32_t getEvents()const;
        uint32_t getREvents()const;
        void setFd(int fd);
        int getFd() const;

        /*Channel 设置不同事件发生时应该调用的回调函数*/
        void setReadHandler(CallBack cb);
        void setWriteHandler(CallBack cb);
        void setErrorHandler(CallBack cb);
        void setConnHandler(CallBack cb);
        /*解析revents*/
        void handleEvents();
        bool hasEventsChanged();
        void updateLastEvents();

        void enableWriting();
        void disableWriting();

        void setRevents(uint32_t events);
        void setEvents(uint32_t events);
    private:
        
        /*channel回调函数*/
        CallBack readHandler_;
        CallBack writeHandler_;
        CallBack errorHandler_;
        CallBack connHandler_;

    private:

        EventLoop* loop_; // 该 Channel 所属的 EventLoop
        // 一个 Channel 由一个 EventLoop 管理
        // 用于事件分发和任务调度
        int fd_;
        uint32_t events_;//当前 Channel 关注的事件
        uint32_t revents_;//epoll返回的实际发生事件
        uint32_t lastEvents_;//
};