#Webserver Arichitecture
## Overview
基于 Linux Epoll的C++ 网络服务器框架。

采用Reactor模型：
EventLoop负责事件循环
Epoll负责IO多路复用
Channel负责事件与回调绑定
TcpConnection管理Tcp生命周期

# 1.Epoll模块
Epoll模块负责对Linux epoll API进行封装，提供事件注册、修改以及删除接口。
该模块内部维护自身的epollFd_ 文件描述符，用于管理内核 epoll实例。
Epoll层封装成员，需要自身的epoll文件，以及用于返回单次检查epoll_wait所活跃的全部channel事件，用vector<channel*>存储，因为事件的产生并不由epoll层发生，只需返回相关地址信息供与
Eventloop资源调度处理层面即可，而unordered_map<int,channel*> channels_成员用于保存文件描述符与 Channel 对象之间的映射关系。通过fd作为key，可以快速定位对应的Channel，从而完成事件的添加、修改以及删除操作。
提供相关接口用于注册，修改以及删除channel所对应的文件描述符fd。析构时处理调用close关闭自身所在的epollFd_
Epoll 不负责管理 Channel 生命周期，仅保存用于事件索引的裸指针，Channel的生命周期由上层对象负责。


# 2.EventLoop模块
EventLoop作为 Reactor 模型中的核心事件调度层，负责协调不同类型事件资源的处理。
其内部主要依赖：
Epoll：负责IO事件检测；
TimerQueue：负责定时任务管理。
EventLoop 本身不直接实现底层事件处理逻辑，而是通过组合成员模块提供的能力完成统一调度。
通过驱动事件循环，获取相关的活跃IO事件，并分发相关事件到对应的Channel,同时管理定时任务添加与取消，控制整个事件循环的生命周期。

通过将事件检测，事件分发以及业务处理分离:
Epoll屏蔽Linux IO多路复用细节
TimerQueue屏蔽定时器实现细节
Channel负责事件和回调的绑定
EventLoop负责整体流程控制

EventLoop::getTimeout
getTimeout()用于计算下一次事件循环等待的超时时间，并将定时器的过期时间转换为epoll_wait() 所需要的毫秒级timeout参数。
由于EventLoop同时负责IO事件和定时任务调度，因此一次事件循环不能无限阻塞等待IO，否则可能导致Timer无法及时触发。

EventLoop::loop
loop()是事件循环的核心函数，负责驱动整个Reactor模型运行。
EventLoop通过不断循环：
获取最近到期的Timer，计算IO等待时间，调用Epoll等待活跃事件，分发Channel事件以及处理到期的定时任务，实现IO事件与定时任务的统一调度。


# 3.Channel模块
Channel是Reactor模型中的事件通道封装，负责管理一个文件描述符fd以及该fd关注的事件，并负责将底层I/O事件分发给对应的回调函数。
Channel内部维护一个指向所属EventLoop的指针，用于表示当前Channel由哪个事件循环管理，成员变量fd_表示该Channel所封装的文件描述符。
close_用于记录Channel当前是否处于关闭状态，避免在关闭后的Channel上继续进行事件注册，修改等操作。
events_表示当前Channel向epoll注册并关注的事件类型，例如读事件，写事件等，revents_表示epoll_wait返回后该fd实际发生的事件，用于事件处理阶段判断触发了哪些操作。
lastEvent_用于保存上一次向epoll注册的事件状态,通过与当前event_对比，可以判断是否需要调用epoll_ctl更新监听事件，避免无效操作。
此外，Channel 维护自身事件状态，用于保证事件注册、修改和处理过程的一致性，避免对无效状态进行操作。
handleEvents()通过解析实际返回的revents通知上层调用相关的handle操作。

Channel只负责：
保存 fd 与事件状态；
管理向 epoll 注册的关注事件；
根据 epoll 返回的事件触发对应回调。
Channel不负责：
不负责实际的数据读写操作；
不负责连接生命周期管理；
不负责业务协议解析；
不负责线程调度和任务执行。
具体的数据处理由上层对象（例如TcpConnection完成，EventLoop负责事件循环调度，而Epoll负责底层事件监听。

# 4.TcpConnection模块
Tcp层是服务器端对客户端连接的抽象封装，负责管理一次Tcp连接的完整生命周期，可理解为两端之间的一条双向通信管道，负责维护连接状态、数据收发以及事件处理。
Tcp层内部包含用于实际网络通信的Socket对象，以及负责事件通知与分发的唯一Channel,其中Socket负责底层fd的资源管理，而Channel负责反馈信息，将fd的I/O事件交由EventLoop调度处理。
同时，Tcp层关联所属的EventLoop，表示该连接由某个事件循环负责调度执行，确保事件处理在对应的线程中完成。
在连接的生命周期方面，Tcp内部维护状态机，用于记录当前的连接状态，以用于保证资源能够正确的释放。
同时Tcp层通过维护inputbuffer,outputbuffer处理非阻塞I/O下的数据交换，input缓冲区用于暂存从Socket中读取的数据，等待上层的解析，而输出缓冲区用作保存暂时复发立即发送的数据，配合EPOLLOUT 事件完成异步发送。
Tcp层持有协议处理器handler,用于处理当前Tcp中的数据流，由于Tcp本身只提供有序可靠的字节流传输，并不关心数据格式，因此格式解析逻辑交由上层Handler处理，实现传输层与应用层的解耦设计。

Tcp层负责:
管理单个Tcp连接生命周期；
封装Socket资源；
维护输入输出缓冲区；
相应Channel分发的读写事件；
调用上层Handler处理收到的数据；

Tcp层不负责:
底层事件的监听；
事件循环调度；
具体协议解析；
业务逻辑处理；

# 5.EventLoopThread模块
主线程调用EventLoop*loop=eventLoopThread.startLoop();创建并启动一个工作线程，
eventfd作为存在于内核的计数器，初始值为0，eventfd counter=0,处于不可读状态,epoll_wait()线程睡眠，而另一个线程write进该fd后，counter=1,可读，epoll发现EPOLLIN，epoll_wait()返回，唤醒，被唤醒后的线程进行handleRead将通知消费










