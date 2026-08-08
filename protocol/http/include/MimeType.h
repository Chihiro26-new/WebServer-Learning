#pragma once
#include <string>
#include <unordered_map>

class MimeType
{
public:
    static std::string get(
        const std::string& path
    );

private:
    static std::string getExtension(
        const std::string& path
    );

    static const std::unordered_map<
        std::string,
        std::string
    > mimeMap_;
};