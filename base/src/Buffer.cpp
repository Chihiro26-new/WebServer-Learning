#include "Buffer.h"
#include <sys/uio.h>
#include <unistd.h>
#include <cstring>
Buffer::Buffer(size_t initialSize)
:
buffer_(kCheapPrepend + initialSize),
readerIndex_(kCheapPrepend),
writerIndex_(kCheapPrepend){}

size_t Buffer::readableBytes() const
{
    return writerIndex_-readerIndex_;
}

size_t Buffer::writableBytes() const
{
    return buffer_.size()-writerIndex_;
}

size_t Buffer::prependableBytes() const
{
    return readerIndex_;
}

void Buffer::shrink()
{
    std::vector<char> buf(kCheapPrepend + readableBytes());
    std::copy(
        begin()+readerIndex_,
        begin()+writerIndex_,
        buf.begin()+kCheapPrepend
    );
    size_t readable = readableBytes();
    buffer_.swap(buf);
    readerIndex_=kCheapPrepend;
    writerIndex_ = readerIndex_ + readable;

}
void Buffer::append(
    const char* data,
    size_t len)
{
    ensureWritable(len);
    std::copy(
        data,
        data+len,
        beginWrite()
    );
    writerIndex_ += len;
}

const char* Buffer::peek() const
{
    return begin() + readerIndex_;
}
void Buffer::append(const std::string& data)
{
    append(data.data(), data.size());
}

void Buffer::ensureWritable(size_t len)
{
    if(writableBytes()>=len)
        return;
    makeSpace(len);
}

void Buffer::makeSpace(size_t len)
{

    if(writableBytes()
        + prependableBytes()
        - kCheapPrepend
        >= len)
    {
        size_t readable = readableBytes();
        std::copy(
            begin()+readerIndex_,
            begin()+writerIndex_,
            begin()+kCheapPrepend
        );
        readerIndex_=kCheapPrepend;
        writerIndex_=readerIndex_+readable;
    }
    else
    {
        buffer_.resize(
            writerIndex_+len
        );
    }
}

size_t Buffer::capacity() const
{
    return buffer_.capacity();
}
size_t Buffer::size() const
{
    return buffer_.size();
}
void Buffer::retrieve(size_t len)
{
    if(len < readableBytes())
    {
        readerIndex_ += len;
    }
    else
    {
        retrieveAll();
    }
}

void Buffer::retrieveAll()
{
    readerIndex_=kCheapPrepend;
    writerIndex_=kCheapPrepend;
}

ssize_t Buffer::readFd(int fd)
{
    char extrabuf[65536];
    struct iovec vec[2];

    vec[0].iov_base =
        begin()+writerIndex_;
    vec[0].iov_len =
        writableBytes();
    vec[1].iov_base =
        extrabuf;
    vec[1].iov_len =
        sizeof(extrabuf);
    ssize_t n =
        readv(fd,vec,2);

    if(n<0)
        return n;
    if(n <= writableBytes())
    {
        writerIndex_ += n;
    }
    else
    {
        size_t writable =
            writableBytes();
        writerIndex_=buffer_.size();
        append(
            extrabuf,
            n-writable
        );
    }
    return n;
}

std::string Buffer::retrieveAllAsString()
{
    std::string result(peek(),readableBytes());
    retrieveAll();
    return result;
}