#include "LogFile.h"
#include <filesystem>
#include <iostream>
LogFile::LogFile()
    :LogFile(LogFileOptions{})
{

}
LogFile::LogFile(const LogFileOptions&options)
    :options_(options)
{
    if (options_.maxFiles == 0)
        throw std::invalid_argument("maxFiles must be greater than 0");
    creatDirctory();
    if (!loadState())
        currentFileIndex_ = options_.maxFiles - 1;

    currentFileIndex_ =
        (currentFileIndex_ + 1) % options_.maxFiles;

    if(!openFile())
    {
        throw std::runtime_error("Open log file failed");
    }
     if (!saveState())
        throw std::runtime_error("Save log state failed");
}
std::string LogFile::getLogFilename()const
{
    return options_.directory+"/"
    +options_.basename+std::to_string(currentFileIndex_)
    +".log";
}
void LogFile::creatDirctory()
{
    std::error_code ec;
    std::filesystem::create_directories(options_.directory,ec);

    if(ec)
    {
       throw std::runtime_error(
        "Create directory failed: " + ec.message()
        );
    }
}
bool LogFile::openFile()
{
    std::string filename=getLogFilename();
    file_.open(filename,std::ios::out|std::ios::app);
    if(!file_.is_open())
        return false;

    std::error_code ec;
    writtenBytes_=std::filesystem::file_size(filename,ec);
    if(ec)
    {
        throw std::runtime_error(
            "Get log file size failed: " + ec.message()
            );
    }
    return true;
}
LogFile::~LogFile()
{
    flush();
}
bool LogFile::append(const char*logline,size_t len)
{
   std::cout << "进入 append\n";
    if (failCount_ > 0) {
        --failCount_;
        std::cout << "故障注入失败，剩余次数: "
              << failCount_ << '\n';
        return false;
    }
    std::cout << "开始真实写入\n";
    if(!file_.is_open()||!file_)
        return false;
    if( writtenBytes_>0&&
        writtenBytes_<=options_.rollSize&&
        len>options_.rollSize-writtenBytes_)
    {
        if(!rollFile())
            return false;
    }
    file_.write(logline,static_cast<std::streamsize>(len));
    std::cout << "真实写入结束\n";
    if(!file_.good())
        return false;
    writtenBytes_+=len;
    return true;
}
bool LogFile::flush()
{
    if(!file_.is_open())
        return false;
    file_.flush();
    return file_.good();
}
bool LogFile::rollFile()
{
    if(file_.is_open())
    {
        file_.flush();
        if(!file_.good())
            return false;
        file_.close();
    }

    currentFileIndex_ = (currentFileIndex_ + 1) % options_.maxFiles;

    file_.open(getLogFilename(),
               std::ios::out | std::ios::trunc);

    if (!file_.is_open())
        return false;

    writtenBytes_ = 0;
    if (!saveState())
        return false;
    return true;
}

bool LogFile::loadState()
{
    std::string statePath = options_.directory + "/logger.state";

    std::ifstream in(statePath);

    if (!in.is_open())
        return false;

    size_t index = 0;

    if (!(in >> index))
        return false;

    if (index >= options_.maxFiles)
        return false;

    currentFileIndex_ = index;
    return true;
}
bool LogFile::saveState()
{
    std::string statePath = options_.directory + "/logger.state";
    std::ofstream out(statePath, std::ios::out | std::ios::trunc);

    if (!out.is_open())
        return false;
    out << currentFileIndex_;
    return out.good();
}