#pragma once
#include <string>
#include <unordered_map>
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
    const std::string& getHeader(const std::string&key)const;
    bool keepAlive() const;//判断长链接
    bool hasBody() const;
    const std::string& getMethod() const;
    const std::string& getPath() const;
    const std::string& getVersion() const;
    const std::string& getBody() const;
    const std::unordered_map<std::string, std::string>& headers() const;

private:
    std::string method_;
    std::string path_;
    std::string version_;
    std::unordered_map<std::string,std::string> headers_;
    std::string body_;
};