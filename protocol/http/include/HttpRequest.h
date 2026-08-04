#pragma once
#include <string>
#include <map>

/*保存解析后的 HTTP 请求信息*/
class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();
    void reset();
    void setMethod(const std::string&method);
    void setPath(const std::string& path);
    void setVersion(const std::string&version);
    void setBody(const std::string&body);
    void addHeader(const std::string& key,const std::string& value);
    std::string getHeader(const std::string& key);

    const std::string& method() const;
    const std::string& path() const;
private:
    std::string method_;
    std::string path_;
    std::string version_;
    std::map<std::string, std::string>headers_;
    std::string body_;
};