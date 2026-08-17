#pragma once
#include <vector>
#include <string>
/**
 * TCP连接的用户态缓冲区
 *
 * Buffer负责管理从Socket读取的字节流，
 * 提供数据缓存、追加、读取、消费等操作。
 *
 * 只负责字节数据的存储和管理。
 *
 * 主要功能：
 * 1. 缓存recv/readv读取的数据
 * 2. 管理可读区、可写区和前置空间
 * 3. 支持数据追加和已处理数据回收
 * 4. 为协议解析器提供连续的数据访问
 *
 * 数据结构：
 *
 * +-----------+---------------+---------------+
 * | prepend   | readable      | writable      |
 * +-----------+---------------+---------------+
 *             ^               ^
 *        readerIndex     writerIndex
 *
 * 生命周期：
 * Socket
 *    |
 * TcpConnection
 *    |
 * Buffer
 *    |
 * HTTP Parser
 */
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;// 预留在数据前的空间,向数据头部添加内容
    static const size_t kInitialSize = 1024;//Buffer默认初始容量
    explicit Buffer(size_t initialSize = kInitialSize);

    size_t readableBytes() const;// 可读数据

    size_t writableBytes() const;// 可写空间

    size_t prependableBytes() const;// 前置空间

    const char* peek() const;// 当前可读位置
    void shrink();
    // 写入数据
    void append(const char* data,size_t len);
    void append(const std::string& data);
    // 消费数据
    void retrieve(size_t len);
    void retrieveAll();// 清空
    std::string retrieveAllAsString();//调试
    // 从fd读取
    ssize_t readFd(int fd);
    size_t capacity() const;
    size_t size()const;
private:
    char* begin()
    {
        return &*buffer_.begin();
    }
    const char* begin() const
    {
        return &*buffer_.begin();
    }
    char* beginWrite()
    {
        return begin()+writerIndex_;
    }
    
    void ensureWritable(size_t len);//确保Buffer空间

    void makeSpace(size_t len);//整理Buffer空间
private:
    std::vector<char> buffer_;

    size_t readerIndex_;// 读位置
    
    size_t writerIndex_;// 写位置
};