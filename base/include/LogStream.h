#pragma once
#include <string>
#include <sstream>
class LogStream
{
public:

    template<typename T>
    LogStream& operator<<(const T& value)
    {
        buffer_ << value;
        return *this;
    }
    std::string str() const
    {
        return buffer_.str();
    }

private:
    std::ostringstream buffer_;
};