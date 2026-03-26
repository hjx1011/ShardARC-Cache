#include "CacheManager.h"
#include "ConcurrencyTest.h"
#include "CacheServer.h"

int main() {
    // 1. 实例化核心门面
    HighAvailableCacheManager manager;

    // 2. 运行本地多线程并发防御测试 (来自 ConcurrencyTest 模块)
    run_concurrency_test(&manager);

    // 3. 启动 HTTP 服务器 (来自 CacheServer 模块)
    start_http_server(&manager);

    return 0;
}