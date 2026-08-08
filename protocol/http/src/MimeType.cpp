#include "MimeType.h"


const std::unordered_map<
    std::string,
    std::string
>
MimeType::mimeMap_ =
{
    {".html", "text/html"},
    {".htm",  "text/html"},

    {".css",  "text/css"},

    {".js",   
     "application/javascript"},

    {".json",
     "application/json"},


    {".png",
     "image/png"},

    {".jpg",
     "image/jpeg"},

    {".jpeg",
     "image/jpeg"},

    {".svg",
     "image/svg+xml"},


    {".txt",
     "text/plain"}
};



std::string MimeType::getExtension(
    const std::string& path
)
{
    auto pos = path.find_last_of('.');


    if(pos == std::string::npos)
    {
        return "";
    }


    return path.substr(pos);
}



std::string MimeType::get(
    const std::string& path
)
{
    auto ext = getExtension(path);


    auto it = mimeMap_.find(ext);


    if(it == mimeMap_.end())
    {
        return "application/octet-stream";
    }


    return it->second;
}