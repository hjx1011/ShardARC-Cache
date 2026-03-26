#include "CacheServer.h"
#include "httplib.h"
#include <iostream>

using namespace httplib;

// 注意：这里不是 main，而是 start_http_server！
void start_http_server(HighAvailableCacheManager* manager) {
    Server svr;
    std::cout << "\n============== 阶段二：启动 KamaCache 分布式服务器 ==============\n";

    svr.Get("/get", [manager](const Request& req, Response& res) {
        if (!req.has_param("key")) {
            res.status = 400;
            res.set_content("Missing 'key' parameter", "text/plain");
            return;
        }
        std::string key = req.get_param_value("key");
        std::string val = manager->get_data(key);
        if (!val.empty()) {
            res.set_content(val, "text/plain");
        } else {
            res.status = 404;
            res.set_content("Cache Miss", "text/plain");
        }
    });

    svr.Post("/put", [manager](const Request& req, Response& res) {
        if (!req.has_param("key") || !req.has_param("val")) {
            res.status = 400;
            res.set_content("Missing key or val", "text/plain");
            return;
        }
        std::string key = req.get_param_value("key");
        std::string val = req.get_param_value("val");
        int ttl = req.has_param("ttl") ? std::stoi(req.get_param_value("ttl")) : 0;
        manager->put_data(key, val, ttl);
        res.set_content("OK", "text/plain");
    });

    std::cout << "🚀 服务器已就绪，监听地址: http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
}