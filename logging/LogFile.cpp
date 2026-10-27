#include "LogFile.h"
LogFile::LogFile(const std::string& filename)
:file_(filename, std::ios::out | std::ios::app)
{
    
}
LogFile::~LogFile()
{
    flush();
}
void LogFile::append(const char*logline,size_t len)
{
    file_.write(logline, len);
}
void LogFile::flush()
{
    file_.flush();
}
bool LogFile::rollFile()
{
    return false;
}