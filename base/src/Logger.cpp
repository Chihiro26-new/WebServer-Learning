#include "Logger.h"
#include "LogStream.h"
#include <stdio.h>
#include <chrono>
Logger::Logger(const SourceFile&file,
        int line,
        Level level)
:file_(file),line_(line),level_(level)
{

    formatTime();
    stream_
        << '['
        << levelToString(level_)
        << "] ";
    stream_.append(file_.data_,
        file_.size_);

    stream_
        << ':'
        << line_
        << ' ';
}
LogStream&Logger::stream()
{
    return stream_;
}

Logger::~Logger()
{
    const auto& buffer = stream_.getBuffer();
    if (output_)
    {
        output_(buffer.data(), buffer.length());
    }
}
const char *Logger::levelToString(Logger::Level level)
{
    switch (level)
    {
        case Logger::Level::DEBUG: return "DEBUG";
        case Logger::Level::INFO:  return "INFO";
        case Logger::Level::WARN:  return "WARN";
        case Logger::Level::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}
void Logger:: formatTime()
{
    auto time=std::chrono::system_clock::now();
    std::time_t t =
        std::chrono::system_clock::to_time_t(time);

    std::tm* tm = std::localtime(&t);

    char buf[32];
    std::strftime(
        buf,
        sizeof(buf),
        "%Y-%m-%d %H:%M:%S",
        tm
    );
    stream_ << '[' << buf << "] ";
}
bool Logger::shouldLog(Level level)
{
    return level >= logLevel_;
}
Logger::Level Logger::logLevel_ = Logger::Level::INFO;
void Logger::setLogLevel(Level level)
{
    logLevel_=level;
}

void defaultOutput(const char *msg,size_t len)
{
    size_t n=fwrite(msg,1,len,stdout);
    (void)n;
}
void defaultFlush()
{
    fflush(stdout);
}
Logger::OutputFunc Logger::output_ = defaultOutput;
Logger::FlushFunc  Logger::flush_=defaultFlush;

void Logger::setOutput(OutputFunc output)
{
    output_ = output;
}

void Logger::setFlush(FlushFunc flush)
{
    flush_=flush;
}