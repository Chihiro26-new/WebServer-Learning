#include "StaticFileHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <fstream>
#include "MimeType.h"
#include <iostream>
#include <stdexcept>
#include <utility>
StaticFileHandler::StaticFileHandler(std::string root)
    : root_(std::move(root))
{
    std::string filePath = root_ + "/index.html";

    std::ifstream file(filePath);

    if (!file)
    {
        throw std::runtime_error(
            "failed to open index.html: " + filePath
        );
    }

    indexHtml_.assign(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}
std::string StaticFileHandler::getFilePath(
    const std::string& url
)
{
    if(root_.back() == '/' && url.front() == '/')
    {
        return root_ + url.substr(1);
    }

    return root_ + url;
}


// void StaticFileHandler::handle(
//     const HttpRequest& request,
//     HttpResponse& response)
// {

//     std::string path=request.path();
//     // std::cout
//     //     << "[StaticFile] request path: "
//     //     << path
//     //     << std::endl;

//     if(path=="/")
//     {
//         path="/index.html";
//     }


//     std::string filePath=getFilePath(path);
//     // std::cout
//     // << "[StaticFile] file path: "
//     // << filePath
//     // << std::endl;
//     std::ifstream file(filePath);


//     if(!file)
//     {
//         // std::cout
//         // << "[StaticFile] open failed"
//         // << std::endl;

//         response.setStatusCode(404);
//         response.addHeader(
//             "Content-Type",
//             "text/plain"
//         );

//         response.setBody(
//             "404 Not Found"
//         );

//         return;
//     }
//     // else
//     // {
//     //     // std::cout
//     //     //     << "[StaticFile] open success"
//     //     //     << std::endl;
//     // }

//     std::string body(
//         (std::istreambuf_iterator<char>(file)),
//         std::istreambuf_iterator<char>()
//     );
//     response.setStatusCode(200);
//     auto mime = MimeType::get(filePath);

//     // std::cout
//     //     << "[StaticFile] mime: "
//     //     << mime
//     //     << std::endl;


//     response.addHeader(
//         "Content-Type",
//     mime
//     );

//     response.setBody(body);
// }


void StaticFileHandler::handle(
    const HttpRequest& request,
    HttpResponse& response)
{
    std::string path = request.path();

    if (path == "/")
    {
        path = "/index.html";
    }

    // index.html 直接从内存返回
    if (path == "/index.html")
    {
        response.setStatusCode(200);

        response.addHeader(
            "Content-Type",
            "text/html"
        );

        response.setBody(indexHtml_);

        return;
    }

    // 其他文件保持原来的逻辑
    std::string filePath = getFilePath(path);

    std::ifstream file(filePath);

    if (!file)
    {
        response.setStatusCode(404);

        response.addHeader(
            "Content-Type",
            "text/plain"
        );

        response.setBody(
            "404 Not Found"
        );

        return;
    }

    std::string body(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    response.setStatusCode(200);

    auto mime = MimeType::get(filePath);

    response.addHeader(
        "Content-Type",
        mime
    );

    response.setBody(body);
}