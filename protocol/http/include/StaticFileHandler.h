#pragma once
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <string>
/**
 * @brief 静态文件处理器
 *
 * 根据 HTTP 请求的路径查找对应的静态文件，
 * 并构造 HTTP 响应。
 *
 * index.html 会在构造时加载到内存中，
 * 后续请求可以直接从内存返回，避免重复访问文件系统。
 */
class StaticFileHandler
{
public:

    /**
     * @brief 构造静态文件处理器
     *
     * @param root 静态文件的根目录
     *
     * @throws std::runtime_error
     *         当 index.html 无法加载时抛出异常
     */
    explicit StaticFileHandler(std::string root);

    /**
     * @brief 处理静态文件请求
     *
     * 根据请求路径查找文件，并将结果写入 HTTP 响应。
     * 如果文件不存在，则返回 404。
     *
     * @param request HTTP 请求
     * @param response HTTP 响应
     */
    void handle(
        const HttpRequest& request,
        HttpResponse& response
    );

private:

    /**
     * @brief 判断字符串是否以指定后缀结尾
     *
     * @param str 待检查的字符串
     * @param suffix 后缀
     * @return 如果 str 以 suffix 结尾则返回 true，否则返回 false
     */
    bool endsWith(
        const std::string& str,
        const std::string& suffix
    );

private:

    // 静态文件根目录
    std::string root_;

    // 缓存的 index.html 内容
    std::string indexHtml_;

    /**
     * @brief 根据 HTTP 路径生成文件系统路径
     *
     * 例如：
     *   /index.html -> ./www/index.html
     *
     * @param url HTTP 请求路径
     * @return 对应的文件系统路径
     */
    std::string getFilePath(
        const std::string& url
    );
};