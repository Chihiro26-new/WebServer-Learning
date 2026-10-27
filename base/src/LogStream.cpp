#include "LogStream.h"
template<int SIZE>
FixedBuffer<SIZE>::FixedBuffer()
    : writeIndex_(0)
{
}

const char digits[] = "9876543210123456789";//正数负数
const char* zero = digits + 9;//0位置

const char digitsHex[] = "0123456789ABCDEF";
static void reverse(char *first_,char *last_)
{
   --last_;
   while(first_<last_)
   {
        char tmp=*first_;
        *first_=*last_;
        *last_=tmp;
        ++first_;
        --last_;
   }
}
template <typename T>
size_t convert(char buf[], T value)
{
    T i=value;
    char *p=buf;    
    do {
        int lsd=static_cast<int>(i%10);
        i/=10;
        *p++=zero[lsd];
    }while (i!=0);
    if(value<0)
        *p++='-';
    *p='\0';
    reverse(buf,p);
    return p-buf;
}

size_t convertHex(char buf[],unsigned long value)
{
    unsigned long i=value;
    char *p=buf;
    do{
        int lsd=static_cast<int>(i%16);
        i/=16;
        *p++ = digitsHex[lsd];
    }while(i!=0);
    *p='\0';
    reverse(buf,p);
    return p-buf;
}
template<int SIZE>
const char*FixedBuffer<SIZE>::data()const
{
    return data_;
}

template<int SIZE>
void FixedBuffer<SIZE>::append(const char *data,size_t len)
{
    if(len>available())
        return;
    memcpy(data_+writeIndex_,data,len);
    writeIndex_+=len;
}

template<int SIZE>
size_t FixedBuffer<SIZE>::available()const
{
    return SIZE-writeIndex_;
}

template<int SIZE>
size_t FixedBuffer<SIZE>::length()const
{
    return writeIndex_;
}

template<int SIZE>
void FixedBuffer<SIZE>::reset()
{
    writeIndex_=0;
}
template<int SIZE>
char *FixedBuffer<SIZE>::current()
{
    return data_+writeIndex_;
}
template<int SIZE>
void FixedBuffer<SIZE>::add(size_t len)
{
    writeIndex_+=len;
}
const LogStream::Buffer&LogStream::getBuffer()const
{
    return buffer_;
}
void LogStream::resetBuffer()
{
    buffer_.reset();
}

template<typename T>
void LogStream::formatInteger(T value)
{
    if (buffer_.available() < kMaxNumericSize)
        return;
    size_t len=convert(buffer_.current(),value);
    buffer_.add(len);
}

LogStream&LogStream::operator<<(short v)
{
    *this<<static_cast<int>(v);
    return *this;
}
LogStream& LogStream::operator<<(unsigned short v) 
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) 
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) 
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) 
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) 
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) 
{
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(const void *p)
{
    unsigned long v=reinterpret_cast<unsigned long>(p);
    if(buffer_.available()>=kMaxNumericSize)
    {
        char *buf=buffer_.current();
        buf[0]='0';
        buf[1]='x';
        size_t len=convertHex(buf+2,v);
        buffer_.add(len+2);
    }
    return *this;
}

LogStream& LogStream::operator<<(double v) 
{
    if(buffer_.available() >= kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(long double v) 
{
    if(buffer_.available() >= kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12Lg", v);
        buffer_.add(len);
    }
    return *this;
}
template class FixedBuffer<4096>;
template class FixedBuffer<4*1024*1024>;