# Chihiro-WebServer

基于 **Linux + C++17** 实现的高性能网络服务器学习项目。

本项目从 Linux Socket 编程和 I/O 多路复用开始，逐步构建基于 **Reactor 模型 + 多线程 Worker Pool** 的网络服务器框架，用于深入学习：

- Linux 网络编程
- TCP/IP
- epoll I/O 多路复用
- Reactor 事件驱动模型
- 多线程网络服务器架构
- C++ RAII 与智能指针
- TCP 连接生命周期管理
- HTTP 协议解析
- 定时器与连接超时管理
- 模块化协议设计
- C++ 工程化设计

项目采用分层、模块化设计，目前已经形成从 **Socket → Acceptor → EventLoop → TcpConnection → ProtocolHandler → HTTP** 的完整处理链路，并支持基于 Worker Pool 的多线程 Reactor 架构。

---

## Features

目前已实现：

### Network

- [x] Linux Socket 网络通信
- [x] 非阻塞 I/O
- [x] epoll I/O 多路复用
- [x] Reactor EventLoop
- [x] Channel 事件封装
- [x] Acceptor TCP 连接接收
- [x] TCP Connection 生命周期管理
- [x] 多 Worker 线程模型

### Buffer & I/O

- [x] 动态 Buffer
- [x] Socket 非阻塞读写
- [x] 输入 / 输出缓冲区
- [x] Partial Read / Partial Write 处理

### Timer

- [x] Timer
- [x] TimerQueue
- [x] 最小堆定时器管理
- [x] EventLoop 与 Timer 集成
- [x] TCP 空闲连接超时
- [x] ConnectionTimerManager

### Protocol

- [x] ProtocolHandler 协议抽象
- [x] Echo 协议
- [x] HTTP 协议
- [x] HTTP Request / Response
- [x] HTTP Parser
- [x] HTTP Keep-Alive
- [x] HTTP 静态文件服务
- [x] MIME Type 判断
- [ ] Redis Protocol

### Engineering

- [x] C++17
- [x] CMake
- [x] RAII 资源管理
- [x] `shared_ptr` / `weak_ptr`
- [x] 模块化设计
- [x] compile_commands.json
- [ ] 完整异步日志系统
- [ ] 压力测试与性能 Benchmark

---

## Architecture

![WebServer Architecture](./Server.png)

