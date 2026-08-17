#pragma once
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <string>
class StaticFileHandler
{
public:

    explicit StaticFileHandler(std::string root);
    void handle(
        const HttpRequest& request,
        HttpResponse& response
    );
    bool endsWith(
    const std::string& str,
    const std::string& suffix
    );
private:

    std::string root_;
    std::string indexHtml_;
    std::string getFilePath(
        const std::string& url
    );
};