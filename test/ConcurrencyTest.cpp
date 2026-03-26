#include "ConcurrencyTest.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// 这是一个仅在当前文件内部使用的辅助函数
void worker_thread(HighAvailableCacheManager* manager, std::string key) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    manager->get_data(key);
}

void run_concurrency_test(HighAvailableCacheManager* manager) {
    std::cout << "\n============== 阶段一：高可用架构防御测试启动 ==============\n";
    
    std::cout << "\n>>> 场景 1：防击穿测试 (10 个线程同时请求未缓存的热点 user_1)\n";
    std::vector<std::thread> threads_1;
    for (int i = 0; i < 10; ++i) {
        threads_1.emplace_back(worker_thread, manager, "user_1");
    }
    for (auto& t : threads_1) t.join();
    
    std::cout << "\n>>> 场景 2：防穿透测试 (10 个线程疯狂请求黑客恶意 key hacker_999)\n";
    std::vector<std::thread> threads_2;
    for (int i = 0; i < 10; ++i) {
        threads_2.emplace_back(worker_thread, manager, "hacker_999");
    }
    for (auto& t : threads_2) t.join();

    std::cout << "\n============== 防御测试圆满结束 ==============\n";
}