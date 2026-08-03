#pragma once
#include "HttpRequest.h"
#include <string>
#include <map>

class HttpRequest
{
    private:
        std::string method_;
        std::string path_;
        std::map<std::string, std::string>headers_;
};